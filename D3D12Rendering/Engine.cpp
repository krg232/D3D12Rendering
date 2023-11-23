#include "Engine.h"

static void EnableDx12DebugLayer()
{
    Microsoft::WRL::ComPtr<ID3D12Debug> debugLayer;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()))))
    {
        debugLayer->EnableDebugLayer();
    }
}

Engine::Engine(HWND hwnd) : _hwnd{ hwnd }
{
	_device = std::make_unique<Device>();
	_commands = std::make_unique<Commands>();
	_swapchain = std::make_unique<SwapChain>();
	_fence = std::make_unique<Fence>();
	_shader = std::make_unique<Shader>();
	_model = std::make_unique<Model>();
	_gpuBuffer = std::make_unique<GPUBuffer>();
	_texture = std::make_unique<Texture>();
	_rootSignature = std::make_unique<RootSignature>();
	_pipelineState = std::make_unique<PipelineState>();
}

void Engine::Init()
{
#ifdef DEBUG
    EnableDx12DebugLayer();
#endif // DEBUG
    _device->InitDevice();
    _dev = _device->GetDevice();
	_commands->InitCommandList(_dev.Get());
    _swapchain->InitSwapChain(_dev.Get(), _device->GetDxgiFactory(), _commands->GetCommandQueue(), _hwnd, WindowWidth, WindowHeight);
	_fence->InitFence(_dev.Get());
    _shader->Compile();
    _model->Load();
	_gpuBuffer->CreateVertexBuffer(_dev.Get(), _model->GetMesh());
	_gpuBuffer->CreateIndexBuffer(_dev.Get(), _model->GetMesh());
	_texture->CreateTexSampler();
	_texture->LoadTexture("Assets//UVCheckerMap01-512.png");
	_gpuBuffer->CreateTextureBuffer(_dev.Get(), _texture->GetRawImage(), _texture->GetTexMetaData());
	_gpuBuffer->CreateConstantBuffer(_dev.Get());
    _pipelineState->SetInputLayout();
	_rootSignature->CreateRootSignature(_dev.Get(), _texture->GetTextureSamplerDesc());
	_pipelineState->CreatePipelineState(_dev.Get(), _rootSignature->GetRootSignature(), _shader->GetVertexShaderBlob(), _shader->GetPixelShaderBlob());
	_device->InitViewPort(WindowWidth, WindowHeight);
}

void Engine::Update()
{
	_swapchain->SetBuckbufferIndex();

    _swapchain->InitBarrierDesc();
	_commands->GetCommandList()->ResourceBarrier(1, _swapchain->GetBarrierDesc());

	_commands->GetCommandList()->SetPipelineState(_pipelineState->GetPipelineState());

    auto rtvHundle = _swapchain->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
    rtvHundle.ptr += _swapchain->GetBackbufferIndex() * _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    _commands->GetCommandList()->OMSetRenderTargets(1, &rtvHundle, false, nullptr);

    _commands->GetCommandList()->SetGraphicsRootSignature(_rootSignature->GetRootSignature());
    _commands->GetCommandList()->SetDescriptorHeaps(1, _gpuBuffer->GetTexDescHeapPtr());
    auto _heapHandle = _gpuBuffer->GetTexDescHeap()->GetGPUDescriptorHandleForHeapStart();
    _commands->GetCommandList()->SetGraphicsRootDescriptorTable(0, _heapHandle);
    _heapHandle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    _commands->GetCommandList()->SetGraphicsRootDescriptorTable(1, _heapHandle);

    _commands->GetCommandList()->RSSetViewports(1, _device->GetViewPort());
    _commands->GetCommandList()->RSSetScissorRects(1, _device->GetScissorRect());

    _commands->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    float clearColor[] = { 0.12f, 0.12f, 0.12f, 1.0f };
    _commands->GetCommandList()->ClearRenderTargetView(rtvHundle, clearColor, 0, nullptr);

    _commands->GetCommandList()->IASetVertexBuffers(0, 1, _gpuBuffer->GetVertexBufferView());
    _commands->GetCommandList()->IASetIndexBuffer(_gpuBuffer->GetIndexBufferView());
    _commands->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);

	_swapchain->SetBarrierState();
    _commands->GetCommandList()->ResourceBarrier(1, _swapchain->GetBarrierDesc());

    _commands->GetCommandList()->Close();

	_commands->ExecuteCommandList();

    _fence->WaitPrevFrame(_commands->GetCommandQueue());

	_commands->ResetCommandList();

	_swapchain->Present();
}
