#pragma once

#include <d3d12.h>
#include <wrl.h>

class Commands
{
public:
	Commands();
	void InitCommandList(ID3D12Device* device);
	ID3D12GraphicsCommandList* GetCommandList() const;
	ID3D12CommandQueue* GetCommandQueue() const;
	void ExecuteCommandList();
	void ResetCommandList();

private:
	HRESULT _result;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
	D3D12_COMMAND_QUEUE_DESC _commandQueueDesc = {};

};
