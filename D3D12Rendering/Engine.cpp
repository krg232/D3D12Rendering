#include "Engine.h"

// デバッグレイヤーの有効化
static void EnableDx12DebugLayer()
{
    Microsoft::WRL::ComPtr<ID3D12Debug> debugLayer;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()))))
    {
        debugLayer->EnableDebugLayer();
    }
}

Engine::Engine(HWND hwnd) : _hwnd{hwnd}
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
    _camera = std::make_unique<Camera>();
}

void Engine::Init()
{
#ifdef DEBUG
    EnableDx12DebugLayer();
#endif // DEBUG
    _device->InitDevice();
    _dev = _device->GetDevice();
    _commands->CreateCommands(_dev.Get());
    _swapchain->CreateSwapChain(_dev.Get(), _device->GetDxgiFactory(), _commands->GetCommandQueue(), _hwnd, WindowWidth, WindowHeight);
    _fence->CreateFence(_dev.Get());

    _shader->Compile();

    _model->Load("Assets/Alicia_FBX/Alicia_solid_Unity.FBX");
    _gpuBuffer->CreateVertexBuffers(_dev.Get(), _model->GetMeshes());
    _gpuBuffer->CreateIndexBuffers(_dev.Get(), _model->GetMeshes());

    _texture->InitTexSampler();
    _texture->LoadTextures(_model->GetMeshes());
    _gpuBuffer->CreateTextureBuffer(_dev.Get(), _texture->GetTextureDatas());

    _camera->InitCamera(DirectX::XMFLOAT3(0, 120, 75), DirectX::XMFLOAT3(0, 120, 0), DirectX::XMFLOAT3(0, 1, 0), 90, WindowWidth, WindowHeight);
    _gpuBuffer->CreateConstantBuffer(_dev.Get());
    _gpuBuffer->SetToMapMatrix(_camera->GetCameraMatrix());

    _gpuBuffer->CreateDeapthBuffer(_dev.Get(), WindowWidth, WindowHeight);

    _pipelineState->SetInputLayout();
    _rootSignature->CreateRootSignature(_dev.Get(), _texture->GetTextureSamplerDesc());
    _pipelineState->CreatePipelineState(_dev.Get(), _rootSignature->GetRootSignature(), _shader->GetVertexShaderBlob(), _shader->GetPixelShaderBlob());

    _device->InitViewPort(WindowWidth, WindowHeight);
}

void Engine::Update()
{
    _swapchain->UpdateBuckbufferIndex();

    // バリアの初期化と遷移
    _swapchain->InitBarrierDesc();
    _commands->GetCommandList()->ResourceBarrier(1, _swapchain->GetBarrierDesc());

    _commands->GetCommandList()->SetPipelineState(_pipelineState->GetPipelineState());

    // RTVの設定
    auto rtvHundle = _swapchain->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
    rtvHundle.ptr += _swapchain->GetBackbufferIndex() * _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    // デプスステンシルの設定
    auto dsvHandle = _gpuBuffer->GetDeapthDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
    _commands->GetCommandList()->OMSetRenderTargets(1, &rtvHundle, true, &dsvHandle);

    // ルートシグネチャ設定
    _commands->GetCommandList()->SetGraphicsRootSignature(_rootSignature->GetRootSignature());

    // ビューポートとシザー矩形の設定
    _commands->GetCommandList()->RSSetViewports(1, _device->GetViewPort());
    _commands->GetCommandList()->RSSetScissorRects(1, _device->GetScissorRect());

    // プリミティブトポロジの背屮
    _commands->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 画面クリア
    float clearColor[] = {0.12f, 0.12f, 0.12f, 1.0f};
    _commands->GetCommandList()->ClearRenderTargetView(rtvHundle, clearColor, 0, nullptr);

    // 深度ステンシルクリア
    _commands->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // テクスチャ用ディスクリプタヒープの設定
    _commands->GetCommandList()->SetDescriptorHeaps(1, _gpuBuffer->GetTexDescHeapPtr());
    // SRV指定用ポインタの設定
    auto _heapHandle = _gpuBuffer->GetTexDescHeap()->GetGPUDescriptorHandleForHeapStart();
    // CBV指定用ポインタの設定
    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle;
    cbvHandle.ptr = _heapHandle.ptr + (_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * _gpuBuffer->GetVertexBufferSize());

    for (int i = 0; i < _gpuBuffer->GetVertexBufferSize(); i++)
    {
        // SRV設定とオフセット調整
        _commands->GetCommandList()->SetGraphicsRootDescriptorTable(0, _heapHandle);
        _heapHandle.ptr += _dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        // CBV設定
        _commands->GetCommandList()->SetGraphicsRootDescriptorTable(1, cbvHandle);

        // ジオメトリ描画
        _commands->GetCommandList()->IASetVertexBuffers(0, 1, _gpuBuffer->GetVertexBufferView(i));
        _commands->GetCommandList()->IASetIndexBuffer(_gpuBuffer->GetIndexBufferView(i));
        _commands->GetCommandList()->DrawIndexedInstanced(_gpuBuffer->GetIndexCount(i), 1, 0, 0, 0);
    }

    // バリアの遷移
    _swapchain->SetBarrierState();
    _commands->GetCommandList()->ResourceBarrier(1, _swapchain->GetBarrierDesc());

    // コマンドクローズ
    _commands->GetCommandList()->Close();

    _commands->ExecuteCommandList();

    _fence->WaitPrevFrame(_commands->GetCommandQueue());

    _commands->ResetCommandList();

    _swapchain->Present();
}
