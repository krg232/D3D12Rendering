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
    _imGuiWrap = std::make_unique<ImGuiWrapper>();
}

void Engine::Init()
{
#ifdef DEBUG
    EnableDx12DebugLayer();
#endif // DEBUG
    _device->InitDevice();
    _commands->CreateCommands(_device->GetDevice());
    _swapchain->CreateSwapChain(_device->GetDevice(), _device->GetDxgiFactory(), _commands->GetCommandQueue(), _hwnd, WindowWidth, WindowHeight);
    _fence->CreateFence(_device->GetDevice());

    _shader->CompileShaders();

    _model->LoadModel("Assets/Alicia_FBX/Alicia_solid_Unity.FBX");
    _gpuBuffer->CreateVertexBuffers(_device->GetDevice(), _model->GetMeshes());
    _gpuBuffer->CreateIndexBuffers(_device->GetDevice(), _model->GetMeshes());

    _texture->InitTexSampler();
    _texture->LoadTextures(_model->GetMeshes());
    _gpuBuffer->CreateTextureBuffer(_device->GetDevice(), _texture->GetTextureDatas());

    _camera->InitCamera(DirectX::XMFLOAT3(0, 120, 75), DirectX::XMFLOAT3(0, 120, 0), DirectX::XMFLOAT3(0, 1, 0), 90, WindowWidth, WindowHeight);
    _gpuBuffer->CreateConstantBuffer(_device->GetDevice());
    _gpuBuffer->UpdateConstBufferMatrix(_camera->GetCameraMatrix());

    _gpuBuffer->CreateDeapthBuffer(_device->GetDevice(), WindowWidth, WindowHeight);

    _pipelineState->SetInputLayout();
    _rootSignature->CreateRootSignature(_device->GetDevice(), _texture->GetTextureSamplerDesc());
    _pipelineState->CreatePipelineState(_device->GetDevice(), _rootSignature->GetRootSignature(), _shader->GetVertexShaderBlob(), _shader->GetPixelShaderBlob());

    _device->InitViewPort(WindowWidth, WindowHeight);

    _imGuiWrap->InitImgui(_hwnd, _device->GetDevice());
}

void Engine::Update()
{
    _swapchain->UpdateBackbufferIndex();

    _imGuiWrap->SetWindow();

    // カメラの回転
    _imGuiWrap->UpdateCameraRotationInput(_camera.get());

    // バリアの初期化と遷移
    _swapchain->InitBarrierDesc();
    _commands->GetCommandList()->ResourceBarrier(1, _swapchain->GetBarrierDesc());

    _commands->GetCommandList()->SetPipelineState(_pipelineState->GetPipelineState());

    // RTVの設定
    auto rtvHundle = _swapchain->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart();
    rtvHundle.ptr += _swapchain->GetBackbufferIndex() * _device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    // デプスステンシルの設定
    auto dsvHandle = _gpuBuffer->GetDepthDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
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

    // 定数バッファの更新
    _gpuBuffer->UpdateConstBufferMatrix(_camera->GetCameraMatrix());

    // テクスチャ用ディスクリプタヒープの設定
    _commands->GetCommandList()->SetDescriptorHeaps(1, _gpuBuffer->GetTexDescHeapPtr());
    // SRV指定用ポインタの設定
    auto _heapHandle = _gpuBuffer->GetTexDescHeap()->GetGPUDescriptorHandleForHeapStart();
    // CBV指定用ポインタの設定
    D3D12_GPU_DESCRIPTOR_HANDLE cbvHandle;
    cbvHandle.ptr = _heapHandle.ptr + (_device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * _gpuBuffer->GetVertexBufferSize());

    for (int i = 0; i < _gpuBuffer->GetVertexBufferSize(); i++)
    {
        // SRV設定とオフセット調整
        _commands->GetCommandList()->SetGraphicsRootDescriptorTable(0, _heapHandle);
        _heapHandle.ptr += _device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        // CBV設定
        _commands->GetCommandList()->SetGraphicsRootDescriptorTable(1, cbvHandle);

        // ジオメトリ描画
        _commands->GetCommandList()->IASetVertexBuffers(0, 1, _gpuBuffer->GetVertexBufferView(i));
        _commands->GetCommandList()->IASetIndexBuffer(_gpuBuffer->GetIndexBufferView(i));
        _commands->GetCommandList()->DrawIndexedInstanced(_gpuBuffer->GetIndexCount(i), 1, 0, 0, 0);
    }

    // imgui用ディスクリプタヒープの設定
    _commands->GetCommandList()->SetDescriptorHeaps(1, _imGuiWrap->GetImguiDescHeapPtr());

    _imGuiWrap->Draw(_commands->GetCommandList());

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
