#include "RootSignature.h"

RootSignature::RootSignature()
{
}

void RootSignature::CreateRootSignature(ID3D12Device* device, D3D12_STATIC_SAMPLER_DESC textureSamplerDesc)
{
    _descRange[0].NumDescriptors = 1;
    _descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    _descRange[0].BaseShaderRegister = 0;
    _descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    _descRange[1].NumDescriptors = 1;
    _descRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    _descRange[1].BaseShaderRegister = 0;
    _descRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    _rootParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    _rootParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    _rootParam[0].DescriptorTable.pDescriptorRanges = &_descRange[0];
    _rootParam[0].DescriptorTable.NumDescriptorRanges = 1;
    _rootParam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    _rootParam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    _rootParam[1].DescriptorTable.pDescriptorRanges = &_descRange[1];
    _rootParam[1].DescriptorTable.NumDescriptorRanges = 1;

    _rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    _rootSignatureDesc.pParameters = _rootParam;
    _rootSignatureDesc.NumParameters = 2;
    _rootSignatureDesc.pStaticSamplers = &textureSamplerDesc;
    _rootSignatureDesc.NumStaticSamplers = 1;

    _result = D3D12SerializeRootSignature(&_rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, _rootSignatureBlob.GetAddressOf(), _errorBlob.GetAddressOf());
    if (FAILED(_result))
    {
        if (_errorBlob != nullptr)
        {
            GetErrorMessage(_errorBlob.Get());
        }
        return;
    }

    _result = device->CreateRootSignature(0, _rootSignatureBlob->GetBufferPointer(), _rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(_rootSignature.GetAddressOf()));
}

ID3D12RootSignature* RootSignature::GetRootSignature() const
{
	return _rootSignature.Get();
}

void RootSignature::GetErrorMessage(ID3DBlob* errorBlob)
{
    std::string error;
    error.resize(errorBlob->GetBufferSize());
    std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), error.begin());
    OutputDebugStringA(error.c_str());
}
