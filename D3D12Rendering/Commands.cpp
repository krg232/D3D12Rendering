#include "Commands.h"

Commands::Commands()
{
}

// コマンドリスト、コマンドアロケーター、コマンドキューの作成
void Commands::CreateCommands(ID3D12Device *device)
{
    const auto commandListType = D3D12_COMMAND_LIST_TYPE_DIRECT;

    _result = device->CreateCommandAllocator(commandListType, IID_PPV_ARGS(_commandAllocator.GetAddressOf()));
    _result = device->CreateCommandList(0, commandListType, _commandAllocator.Get(), nullptr, IID_PPV_ARGS(_commandList.GetAddressOf()));

    _commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    _commandQueueDesc.NodeMask = 0;
    _commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    _commandQueueDesc.Type = commandListType;

    _result = device->CreateCommandQueue(&_commandQueueDesc, IID_PPV_ARGS(_commandQueue.GetAddressOf()));
}

ID3D12GraphicsCommandList *Commands::GetCommandList() const
{
    return _commandList.Get();
}

ID3D12CommandQueue *Commands::GetCommandQueue() const
{
    return _commandQueue.Get();
}

void Commands::ExecuteCommandList()
{
    ID3D12CommandList *commandLists[] = {_commandList.Get()};
    _commandQueue->ExecuteCommandLists(1, commandLists);
}

void Commands::ResetCommandList()
{
    _commandAllocator->Reset();
    _commandList->Reset(_commandAllocator.Get(), nullptr);
}
