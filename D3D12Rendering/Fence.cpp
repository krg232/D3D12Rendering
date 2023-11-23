#include "Fence.h"

Fence::Fence()
{
}

void Fence::InitFence(ID3D12Device* device)
{
	_result = device->CreateFence(_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(_fence.GetAddressOf()));
}

void Fence::WaitPrevFrame(ID3D12CommandQueue* commandQueue)
{
    commandQueue->Signal(_fence.Get(), ++_fenceVal);
    if (_fence->GetCompletedValue() != _fenceVal)
    {
        auto event = CreateEvent(nullptr, false, false, nullptr);
        _fence->SetEventOnCompletion(_fenceVal, event);
        WaitForSingleObject(event, INFINITE);
        CloseHandle(event);
    }
}
