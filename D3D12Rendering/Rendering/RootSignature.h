#pragma once

#include <string>
#include <d3d12.h>
#include <wrl.h>

class RootSignature
{
public:
	RootSignature();
	void CreateRootSignature(ID3D12Device *device, D3D12_STATIC_SAMPLER_DESC textureSamplerDesc);
	ID3D12RootSignature *GetRootSignature() const;

private:
	void GetErrorMessage(ID3DBlob *errorBlob); // エラーメッセージを取得する

	HRESULT _result;

	Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;
	D3D12_DESCRIPTOR_RANGE _descRange[2] = {};
	D3D12_ROOT_PARAMETER _rootParam[2] = {};
	D3D12_ROOT_SIGNATURE_DESC _rootSignatureDesc = {};
	Microsoft::WRL::ComPtr<ID3DBlob> _rootSignatureBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> _errorBlob;
};
