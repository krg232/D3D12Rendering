#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <d3dx12.h>
#include "Struct.h"

class GPUBuffer
{
public:
	GPUBuffer();
	void CreateVertexBuffer(ID3D12Device* device, Mesh mesh);
	void CreateIndexBuffer(ID3D12Device* device, Mesh mesh);
	void CreateTextureBuffer(ID3D12Device* device, DirectX::Image *rawImage, DirectX::TexMetadata texMetaData);
	void CreateConstantBuffer(ID3D12Device* device);
	ID3D12DescriptorHeap* GetTexDescHeap() const;
	ID3D12DescriptorHeap* const* GetTexDescHeapPtr() const;
	const D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView() const;
	const D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView() const;

private:
	HRESULT _result;

	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {};
	D3D12_HEAP_PROPERTIES _vertexHeapProperty = {};
	D3D12_RESOURCE_DESC _vertexResourceDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer;
	Vertex* _vertMap;

	Microsoft::WRL::ComPtr<ID3D12Resource> _indexBuffer;
	USHORT* _indexMap;
	D3D12_INDEX_BUFFER_VIEW _indexBufferView = {};

	D3D12_HEAP_PROPERTIES _textureHeapProperty = {};
	D3D12_RESOURCE_DESC _textureResourceDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _textureBuffer;
	D3D12_DESCRIPTOR_HEAP_DESC _textureHeapDesc = {};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _texDescHeap;
	D3D12_SHADER_RESOURCE_VIEW_DESC _shaderResourceView = {};
	D3D12_CPU_DESCRIPTOR_HANDLE _texDescHeapHandle = {};

	D3D12_CONSTANT_BUFFER_VIEW_DESC _cbvDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _constBuffer;
	std::shared_ptr<DirectX::XMMATRIX> _mapMatrix;
	DirectX::XMMATRIX _matrix;

};
