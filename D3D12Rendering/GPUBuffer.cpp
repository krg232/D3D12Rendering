#include "GPUBuffer.h"

GPUBuffer::GPUBuffer()
{
}

void GPUBuffer::CreateVertexBuffers(ID3D12Device *device, std::vector<Mesh> meshs)
{
    _vertexHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    _vertexHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    _vertexHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    _vertexBuffers.resize(meshs.size());

    for (int i = 0; i < meshs.size(); i++)
    {
        // 頂点バッファの作成
        _vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        _vertexResourceDesc.Width = meshs.at(i).vertices.size() * sizeof(Vertex);
        _vertexResourceDesc.Height = 1;
        _vertexResourceDesc.DepthOrArraySize = 1;
        _vertexResourceDesc.MipLevels = 1;
        _vertexResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        _vertexResourceDesc.SampleDesc.Count = 1;
        _vertexResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        _vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        _result = device->CreateCommittedResource(&_vertexHeapProperty, D3D12_HEAP_FLAG_NONE, &_vertexResourceDesc,
                                                  D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_vertexBuffer.GetAddressOf()));

        // GPU側に頂点データをマップ
        _result = _vertexBuffer->Map(0, nullptr, (void **)&_vertMap);
        std::copy(std::begin(meshs.at(i).vertices), std::end(meshs.at(i).vertices), _vertMap);
        _vertexBuffer->Unmap(0, nullptr);

        _vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
        _vertexBufferView.SizeInBytes = static_cast<int>(meshs.at(i).vertices.size() * sizeof(Vertex));
        _vertexBufferView.StrideInBytes = sizeof(Vertex);

        _vertexResourceDesc.Width = meshs.at(i).indices.size() * sizeof(std::int32_t);

        _vertexBuffers.at(i)._vertexBuffer = _vertexBuffer;
        _vertexBuffers.at(i)._vertexBufferView = _vertexBufferView;
    }
}

void GPUBuffer::CreateIndexBuffers(ID3D12Device *device, std::vector<Mesh> meshs)
{
    _vertexHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    _vertexHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    _vertexHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    _indexBuffers.resize(meshs.size());

    for (int i = 0; i < meshs.size(); i++)
    {
        // インデックスバッファの作成
        _indexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        _indexResourceDesc.Width = meshs.at(i).indices.size() * sizeof(unsigned short);
        _indexResourceDesc.Height = 1;
        _indexResourceDesc.DepthOrArraySize = 1;
        _indexResourceDesc.MipLevels = 1;
        _indexResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        _indexResourceDesc.SampleDesc.Count = 1;
        _indexResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        _indexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        _result = device->CreateCommittedResource(&_vertexHeapProperty, D3D12_HEAP_FLAG_NONE, &_indexResourceDesc,
                                                  D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_indexBuffer.GetAddressOf()));

        // GPU側にインデックスデータをマップ
        _result = _indexBuffer->Map(0, nullptr, (void **)&_indexMap);
        std::copy(std::begin(meshs.at(i).indices), std::end(meshs.at(i).indices), _indexMap);
        _indexBuffer->Unmap(0, nullptr);

        _indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
        _indexBufferView.SizeInBytes = meshs.at(i).indices.size() * sizeof(unsigned short);
        _indexBufferView.Format = DXGI_FORMAT_R16_UINT;

        _indexBuffers.at(i)._indexBuffer = _indexBuffer;
        _indexBuffers.at(i)._indexBufferView = _indexBufferView;
        _indexBuffers.at(i)._indexCount = meshs.at(i).indices.size();
    }
}

void GPUBuffer::CreateTextureBuffer(ID3D12Device *device, const std::vector<TextureData> &texDatas)
{
    _textureBuffers.resize(texDatas.size());

    // ディスクリプタヒープの作成
    _textureHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    _textureHeapDesc.NodeMask = 0;
    _textureHeapDesc.NumDescriptors = texDatas.size();
    _textureHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    _result = device->CreateDescriptorHeap(&_textureHeapDesc, IID_PPV_ARGS(_texDescHeap.GetAddressOf()));

    // ヒープの先頭ハンドル
    _texDescHeapHandle = _texDescHeap->GetCPUDescriptorHandleForHeapStart();

    _textureHeapProperty.Type = D3D12_HEAP_TYPE_CUSTOM;
    _textureHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    _textureHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    _textureHeapProperty.CreationNodeMask = 0;
    _textureHeapProperty.VisibleNodeMask = 0;

    for (int i = 0; i < texDatas.size(); i++)
    {
        // テクスチャバッファの作成
        _textureResourceDesc.Format = texDatas.at(i).texMetaData.format;
        _textureResourceDesc.Width = texDatas.at(i).texMetaData.width;
        _textureResourceDesc.Height = texDatas.at(i).texMetaData.height;
        _textureResourceDesc.DepthOrArraySize = texDatas.at(i).texMetaData.arraySize;
        _textureResourceDesc.SampleDesc.Count = 1;
        _textureResourceDesc.SampleDesc.Quality = 0;
        _textureResourceDesc.MipLevels = texDatas.at(i).texMetaData.mipLevels;
        _textureResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(texDatas.at(i).texMetaData.dimension);
        _textureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        _textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        _result = device->CreateCommittedResource(&_textureHeapProperty, D3D12_HEAP_FLAG_NONE, &_textureResourceDesc,
                                                  D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(_textureBuffer.GetAddressOf()));

        _result = _textureBuffer->WriteToSubresource(0, nullptr, texDatas.at(i).rawImage.pixels, texDatas.at(i).rawImage.rowPitch, texDatas.at(i).rawImage.slicePitch);

        _shaderResourceView.Format = texDatas.at(i).texMetaData.format;
        _shaderResourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        _shaderResourceView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        _shaderResourceView.Texture2D.MipLevels = 1;

        // SRVをヒープに順番に登録
        device->CreateShaderResourceView(_textureBuffer.Get(), &_shaderResourceView, _texDescHeapHandle);
        _texDescHeapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        _textureBuffers.at(i)._textureBuffer = _textureBuffer;
        _textureBuffers.at(i)._srv = _shaderResourceView;
    }
}

void GPUBuffer::CreateConstantBuffer(ID3D12Device *device)
{
    _matrix = DirectX::XMMatrixIdentity();
    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resoueceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(_matrix) + 0xff) & ~0xff); // 必要バイト数を256アライメントする

    // 定数バッファの作成
    _result = device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resoueceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(_constBuffer.GetAddressOf()));

    // GPU側にマップ
    _result = _constBuffer->Map(0, nullptr, (void **)&_mapMatrix);
    *_mapMatrix = _matrix;

    // ビューの作成と登録
    _cbvDesc.BufferLocation = _constBuffer->GetGPUVirtualAddress();
    _cbvDesc.SizeInBytes = _constBuffer->GetDesc().Width;
    device->CreateConstantBufferView(&_cbvDesc, _texDescHeapHandle);
}

void GPUBuffer::CreateDeapthBuffer(ID3D12Device *device, int width, int height)
{
    _deapthResorceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    _deapthResorceDesc.Width = width;
    _deapthResorceDesc.Height = height;
    _deapthResorceDesc.DepthOrArraySize = 1;
    _deapthResorceDesc.Format = DXGI_FORMAT_D32_FLOAT;
    _deapthResorceDesc.SampleDesc.Count = 1;
    _deapthResorceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    _deapthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
    _deapthHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    _deapthHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

    // 深度クリア値の設定
    _deapthCrearValue.DepthStencil.Depth = 1.0f;
    _deapthCrearValue.Format = DXGI_FORMAT_D32_FLOAT;

    // 深度バッファの作成
    _result = device->CreateCommittedResource(&_deapthHeapProp, D3D12_HEAP_FLAG_NONE, &_deapthResorceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &_deapthCrearValue, IID_PPV_ARGS(_deapthBuffer.GetAddressOf()));

    _deapthDescriptorHeapDesc.NumDescriptors = 1;
    _deapthDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    _result = device->CreateDescriptorHeap(&_deapthDescriptorHeapDesc, IID_PPV_ARGS(_deapthDescriptorHeap.GetAddressOf()));

    // デプスステンシルビューの作成
    _stencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    _stencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    _stencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;
    device->CreateDepthStencilView(_deapthBuffer.Get(), &_stencilViewDesc, _deapthDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
}

int GPUBuffer::GetVertexBufferSize() const
{
    return _vertexBuffers.size();
}

int GPUBuffer::GetTextureBufferSize() const
{
    return _textureBuffers.size();
}

ID3D12DescriptorHeap *GPUBuffer::GetTexDescHeap() const
{
    return _texDescHeap.Get();
}

ID3D12DescriptorHeap *const *GPUBuffer::GetTexDescHeapPtr() const
{
    return _texDescHeap.GetAddressOf();
}

const D3D12_VERTEX_BUFFER_VIEW *GPUBuffer::GetVertexBufferView(int index) const
{
    return &(_vertexBuffers.at(index)._vertexBufferView);
}

const D3D12_INDEX_BUFFER_VIEW *GPUBuffer::GetIndexBufferView(int index) const
{
    return &(_indexBuffers.at(index)._indexBufferView);
}

int GPUBuffer::GetIndexCount(int index) const
{
    return _indexBuffers.at(index)._indexCount;
}

void GPUBuffer::SetToMapMatrix(DirectX::XMMATRIX mat)
{
    *_mapMatrix = mat;
}

ID3D12DescriptorHeap *GPUBuffer::GetDeapthDescriptorHeap() const
{
    return _deapthDescriptorHeap.Get();
}
