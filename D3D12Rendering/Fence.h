#pragma once

#include <d3d12.h>
#include <wrl.h>

class Fence
{
public:
	Fence();
	void InitFence(ID3D12Device* device);
	void WaitPrevFrame(ID3D12CommandQueue* commandQueue);

private:
	HRESULT _result;
	Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
	UINT64 _fenceVal = 0;

};
