#include "SwapChain.h"

SwapChain::SwapChain() : backbufferIndex(0)
{
}

void SwapChain::CreateSwapChain(ID3D12Device *device, IDXGIFactory6 *dxgiFactory, ID3D12CommandQueue *commandQueue, HWND hwnd, int width, int height)
{
    _swapchainDesc.Width = width;
    _swapchainDesc.Height = height;
    _swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    _swapchainDesc.Stereo = false;
    _swapchainDesc.SampleDesc.Count = 1;
    _swapchainDesc.SampleDesc.Quality = 0;
    _swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
    _swapchainDesc.BufferCount = 2;
    _swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
    _swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    _swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    _swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    // 　スワップチェーンの作成
    _result = dxgiFactory->CreateSwapChainForHwnd(commandQueue, hwnd, &_swapchainDesc, nullptr, nullptr, reinterpret_cast<IDXGISwapChain1 **>(_swapchain.GetAddressOf()));

    // RTV用ディスクリプタヒープの作成
    _heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    _heapDesc.NodeMask = 0;
    _heapDesc.NumDescriptors = 2;
    _heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    _result = device->CreateDescriptorHeap(&_heapDesc, IID_PPV_ARGS(_rtvHeaps.GetAddressOf()));

    // バックバッファとRTVを紐付ける
    _backBuffers.resize(_swapchainDesc.BufferCount);
    for (int i = 0; i < _swapchainDesc.BufferCount; i++)
    {
        _result = _swapchain->GetBuffer(i, IID_PPV_ARGS(_backBuffers.at(i).GetAddressOf()));
        D3D12_CPU_DESCRIPTOR_HANDLE handle = _rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        handle.ptr += i * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        device->CreateRenderTargetView(_backBuffers.at(i).Get(), nullptr, handle);
    }
}

void SwapChain::InitBarrierDesc()
{
    _barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    _barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    _barrierDesc.Transition.pResource = _backBuffers[backbufferIndex].Get();
    _barrierDesc.Transition.Subresource = 0;
    _barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    _barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
}

void SwapChain::UpdateBackbufferIndex()
{
    backbufferIndex = _swapchain->GetCurrentBackBufferIndex();
}

void SwapChain::SetBarrierState()
{
    _barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    _barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
}

D3D12_RESOURCE_BARRIER *SwapChain::GetBarrierDesc()
{
    return &_barrierDesc;
}

ID3D12DescriptorHeap *SwapChain::GetRtvHeap() const
{
    return _rtvHeaps.Get();
}

int SwapChain::GetBackbufferIndex() const
{
    return backbufferIndex;
}

void SwapChain::Present()
{
    _swapchain->Present(1, 0);
}
