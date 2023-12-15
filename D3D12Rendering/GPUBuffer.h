#pragma once

#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <d3dx12.h>
#include "Struct.h"

struct VertexBuffer
{
	Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;
};

struct IndexBuffer
{
	Microsoft::WRL::ComPtr<ID3D12Resource> _indexBuffer;
	D3D12_INDEX_BUFFER_VIEW _indexBufferView;
	int _indexCount;
};

struct TextureBuffer
{
	Microsoft::WRL::ComPtr<ID3D12Resource> _textureBuffer;
	D3D12_SHADER_RESOURCE_VIEW_DESC _srv;
};

class GPUBuffer
{
public:
	GPUBuffer();
	void CreateVertexBuffers(ID3D12Device *device, std::vector<Mesh> mesh);
	void CreateIndexBuffers(ID3D12Device *device, std::vector<Mesh> mesh);
	void CreateTextureBuffer(ID3D12Device *device, const std::vector<TextureData> &texDatas);
	void CreateConstantBuffer(ID3D12Device *device);
	void CreateDeapthBuffer(ID3D12Device *device, int width, int height);
	int GetVertexBufferSize() const;
	int GetTextureBufferSize() const;
	ID3D12DescriptorHeap *GetTexDescHeap() const;
	ID3D12DescriptorHeap *const *GetTexDescHeapPtr() const;
	const D3D12_VERTEX_BUFFER_VIEW *GetVertexBufferView(int index) const;
	const D3D12_INDEX_BUFFER_VIEW *GetIndexBufferView(int index) const;
	int GetIndexCount(int index) const;
	void SetToMapMatrix(DirectX::XMMATRIX mat);
	ID3D12DescriptorHeap *GetDeapthDescriptorHeap() const;

private:
	HRESULT _result;

	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {};
	D3D12_HEAP_PROPERTIES _vertexHeapProperty = {};
	D3D12_RESOURCE_DESC _vertexResourceDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer;
	std::vector<VertexBuffer> _vertexBuffers;
	Vertex *_vertMap;

	D3D12_RESOURCE_DESC _indexResourceDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _indexBuffer;
	std::vector<IndexBuffer> _indexBuffers;
	USHORT *_indexMap;
	D3D12_INDEX_BUFFER_VIEW _indexBufferView = {};

	D3D12_HEAP_PROPERTIES _textureHeapProperty = {};
	D3D12_RESOURCE_DESC _textureResourceDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _textureBuffer;
	D3D12_DESCRIPTOR_HEAP_DESC _textureHeapDesc = {};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _texDescHeap;
	D3D12_SHADER_RESOURCE_VIEW_DESC _shaderResourceView = {};
	D3D12_CPU_DESCRIPTOR_HANDLE _texDescHeapHandle = {};
	std::vector<TextureBuffer> _textureBuffers;

	D3D12_CONSTANT_BUFFER_VIEW_DESC _cbvDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _constBuffer;
	std::shared_ptr<DirectX::XMMATRIX> _mapMatrix;
	DirectX::XMMATRIX _matrix;

	D3D12_RESOURCE_DESC _deapthResorceDesc = {};
	D3D12_HEAP_PROPERTIES _deapthHeapProp = {};
	D3D12_CLEAR_VALUE _deapthCrearValue = {};
	D3D12_DESCRIPTOR_HEAP_DESC _deapthDescriptorHeapDesc = {};
	D3D12_DEPTH_STENCIL_VIEW_DESC _stencilViewDesc = {};
	Microsoft::WRL::ComPtr<ID3D12Resource> _deapthBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _deapthDescriptorHeap;
};
