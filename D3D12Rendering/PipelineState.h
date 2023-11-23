#pragma once

#include <d3d12.h>
#include <wrl.h>

class PipelineState
{
public:
	PipelineState();
	void SetInputLayout();
	void CreatePipelineState(ID3D12Device* device, ID3D12RootSignature* rootSignature, ID3DBlob* vsBlob, ID3DBlob* psBlob);
	ID3D12PipelineState* GetPipelineState() const;

private:
	HRESULT _result;

	D3D12_INPUT_ELEMENT_DESC _inputLayout[2];

	D3D12_RENDER_TARGET_BLEND_DESC _renderTargetBlendDesc = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _graphicsPipelineStateDesc = {};
	Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;

};
