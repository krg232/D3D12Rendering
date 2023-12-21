#include "PipelineState.h"

PipelineState::PipelineState()
{
}

void PipelineState::SetInputLayout()
{
    _inputLayout[0] = {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
    _inputLayout[1] = {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
    _inputLayout[2] = {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
    _inputLayout[3] = {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
}

void PipelineState::CreatePipelineState(ID3D12Device *device, ID3D12RootSignature *rootSignature, ID3DBlob *vsBlob, ID3DBlob *psBlob)
{
    // レンダーターゲットのブレンド状態の設定
    _renderTargetBlendDesc.BlendEnable = true;
    _renderTargetBlendDesc.LogicOpEnable = false;
    _renderTargetBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    _renderTargetBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    _renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
    _renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
    _renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
    _renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    _renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    // グラフィックスパイプラインステートの設定
    _graphicsPipelineStateDesc.pRootSignature = rootSignature;
    _graphicsPipelineStateDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    _graphicsPipelineStateDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
    _graphicsPipelineStateDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
    _graphicsPipelineStateDesc.PS.BytecodeLength = psBlob->GetBufferSize();
    _graphicsPipelineStateDesc.BlendState.RenderTarget[0] = _renderTargetBlendDesc;
    _graphicsPipelineStateDesc.BlendState.AlphaToCoverageEnable = true;
    _graphicsPipelineStateDesc.BlendState.IndependentBlendEnable = false;
    _graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    _graphicsPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    _graphicsPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    _graphicsPipelineStateDesc.RasterizerState.FrontCounterClockwise = false;
    _graphicsPipelineStateDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    _graphicsPipelineStateDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    _graphicsPipelineStateDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    _graphicsPipelineStateDesc.RasterizerState.DepthClipEnable = true;
    _graphicsPipelineStateDesc.RasterizerState.MultisampleEnable = false;
    _graphicsPipelineStateDesc.RasterizerState.AntialiasedLineEnable = false;
    _graphicsPipelineStateDesc.RasterizerState.ForcedSampleCount = false;
    _graphicsPipelineStateDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    _graphicsPipelineStateDesc.DepthStencilState.DepthEnable = true;
    _graphicsPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    _graphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
    _graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    _graphicsPipelineStateDesc.DepthStencilState.StencilEnable = false;
    _graphicsPipelineStateDesc.InputLayout.pInputElementDescs = _inputLayout;
    _graphicsPipelineStateDesc.InputLayout.NumElements = _countof(_inputLayout);
    _graphicsPipelineStateDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    _graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    _graphicsPipelineStateDesc.NumRenderTargets = 1;
    _graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    _graphicsPipelineStateDesc.SampleDesc.Count = 1;
    _graphicsPipelineStateDesc.SampleDesc.Quality = 0;
    _graphicsPipelineStateDesc.NodeMask = 0;

    _result = device->CreateGraphicsPipelineState(&_graphicsPipelineStateDesc, IID_PPV_ARGS(_pipelineState.GetAddressOf()));
}

ID3D12PipelineState *PipelineState::GetPipelineState() const
{
    return _pipelineState.Get();
}
