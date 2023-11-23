#include "GPUBuffer.h"

GPUBuffer::GPUBuffer()
{
}

void GPUBuffer::CreateVertexBuffer(ID3D12Device* device, Mesh mesh)
{
    _vertexHeapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    _vertexHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    _vertexHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

        _vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        _vertexResourceDesc.Width = mesh.vertices.size() * sizeof(Vertex);
        _vertexResourceDesc.Height = 1;
        _vertexResourceDesc.DepthOrArraySize = 1;
        _vertexResourceDesc.MipLevels = 1;
        _vertexResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        _vertexResourceDesc.SampleDesc.Count = 1;
        _vertexResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        _vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        _result = device->CreateCommittedResource(&_vertexHeapProperty, D3D12_HEAP_FLAG_NONE, &_vertexResourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_vertexBuffer.GetAddressOf()));

        _result = _vertexBuffer->Map(0, nullptr, (void**)&_vertMap);
        std::copy(std::begin(mesh.vertices), std::end(mesh.vertices), _vertMap);
        _vertexBuffer->Unmap(0, nullptr);

        _vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
        _vertexBufferView.SizeInBytes = static_cast<int>(mesh.vertices.size() * sizeof(Vertex));
        _vertexBufferView.StrideInBytes = sizeof(Vertex);

        _vertexResourceDesc.Width = mesh.indices.size() * sizeof(std::int32_t);

}

void GPUBuffer::CreateIndexBuffer(ID3D12Device* device, Mesh mesh)
{
    _result = device->CreateCommittedResource(&_vertexHeapProperty, D3D12_HEAP_FLAG_NONE, &_vertexResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(_indexBuffer.GetAddressOf()));

    _result = _indexBuffer->Map(0, nullptr, (void**)&_indexMap);
    std::copy(std::begin(mesh.indices), std::end(mesh.indices), _indexMap);
    _indexBuffer->Unmap(0, nullptr);

    _indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
    _indexBufferView.SizeInBytes = mesh.indices.size() * sizeof(unsigned short);
    _indexBufferView.Format = DXGI_FORMAT_R16_UINT;

}

void GPUBuffer::CreateTextureBuffer(ID3D12Device* device, DirectX::Image *rawImage, DirectX::TexMetadata texMetaData)
{
    _textureHeapProperty.Type = D3D12_HEAP_TYPE_CUSTOM;
    _textureHeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    _textureHeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    _textureHeapProperty.CreationNodeMask = 0;
    _textureHeapProperty.VisibleNodeMask = 0;

    _textureResourceDesc.Format = texMetaData.format;
    _textureResourceDesc.Width = texMetaData.width;
    _textureResourceDesc.Height = texMetaData.height;
    _textureResourceDesc.DepthOrArraySize = texMetaData.arraySize;
    _textureResourceDesc.SampleDesc.Count = 1;
    _textureResourceDesc.SampleDesc.Quality = 0;
    _textureResourceDesc.MipLevels = texMetaData.mipLevels;
    _textureResourceDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(texMetaData.dimension);
    _textureResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    _textureResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    _result = device->CreateCommittedResource(&_textureHeapProperty, D3D12_HEAP_FLAG_NONE, &_textureResourceDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, nullptr, IID_PPV_ARGS(_textureBuffer.GetAddressOf()));

    _result = _textureBuffer->WriteToSubresource(0, nullptr, rawImage->pixels, rawImage->rowPitch, rawImage->slicePitch);

    _textureHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    _textureHeapDesc.NodeMask = 0;
    _textureHeapDesc.NumDescriptors = 2;
    _textureHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    _result = device->CreateDescriptorHeap(&_textureHeapDesc, IID_PPV_ARGS(_texDescHeap.GetAddressOf()));

    _shaderResourceView.Format = texMetaData.format;
    _shaderResourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    _shaderResourceView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    _shaderResourceView.Texture2D.MipLevels = 1;

    _texDescHeapHandle = _texDescHeap->GetCPUDescriptorHandleForHeapStart();
    device->CreateShaderResourceView(_textureBuffer.Get(), &_shaderResourceView, _texDescHeapHandle);
    _texDescHeapHandle.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void GPUBuffer::CreateConstantBuffer(ID3D12Device* device)
{
    _matrix = DirectX::XMMatrixIdentity();
    auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto resoueceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(_matrix) + 0xff) & ~0xff);
    _result = device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resoueceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(_constBuffer.GetAddressOf()));

    DirectX::XMMATRIX _viewMatrix;
    DirectX::XMMATRIX _projectionMatrix;
    {
        using DirectX::XMLoadFloat3;

        ViewMatrix _viewMat;
        _viewMat.eyePos = DirectX::XMFLOAT3(0, 0, -5);
        _viewMat.forcusPos = DirectX::XMFLOAT3(0, 0, 0);
        _viewMat.upDirection = DirectX::XMFLOAT3(0, 1, 0);


        _viewMatrix = DirectX::XMMatrixLookAtLH(
            XMLoadFloat3(&_viewMat.eyePos),
            XMLoadFloat3(&_viewMat.forcusPos),
            XMLoadFloat3(&_viewMat.upDirection));
        _projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(90),
            static_cast<float>(1920) / static_cast<float>(1080),
            1.0f,
            10.0f);
    }

    _result = _constBuffer->Map(0, nullptr, (void**)&_mapMatrix);
    *_mapMatrix = _matrix * _viewMatrix * _projectionMatrix;

    _cbvDesc.BufferLocation = _constBuffer->GetGPUVirtualAddress();
    _cbvDesc.SizeInBytes = _constBuffer->GetDesc().Width;
    device->CreateConstantBufferView(&_cbvDesc, _texDescHeapHandle);
}

ID3D12DescriptorHeap* GPUBuffer::GetTexDescHeap() const
{
	return _texDescHeap.Get();
}

ID3D12DescriptorHeap*const* GPUBuffer::GetTexDescHeapPtr() const
{
	return _texDescHeap.GetAddressOf();
}

const D3D12_VERTEX_BUFFER_VIEW* GPUBuffer::GetVertexBufferView() const
{
	return &_vertexBufferView;
}

const D3D12_INDEX_BUFFER_VIEW* GPUBuffer::GetIndexBufferView() const
{
    return &_indexBufferView;
}
