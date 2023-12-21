#pragma once

#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

class SwapChain
{
public:
	SwapChain();
	void CreateSwapChain(ID3D12Device *device, IDXGIFactory6 *dxgiFactory, ID3D12CommandQueue *commandQueue, HWND hwnd, int width, int height);
	void InitBarrierDesc();
	void UpdateBackbufferIndex();
	void SetBarrierState(); // バリアをPresentにする
	D3D12_RESOURCE_BARRIER *GetBarrierDesc();
	ID3D12DescriptorHeap *GetRtvHeap() const;
	int GetBackbufferIndex() const;
	void Present(); // バックバッファをフリップ

private:
	HRESULT _result;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapchain;
	DXGI_SWAP_CHAIN_DESC1 _swapchainDesc = {};
	D3D12_DESCRIPTOR_HEAP_DESC _heapDesc = {};
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeaps;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> _backBuffers;
	unsigned int backbufferIndex;

	D3D12_RESOURCE_BARRIER _barrierDesc = {};
};
