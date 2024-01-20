#include "Shader.h"

Shader::Shader()
{
    _vsFilename = "Shader//BasicVertexShader.hlsl";
    _psFilename = "Shader//BasicPixelShader.hlsl";
}

void Shader::CompileShaders()
{
    CompileShader(StrToWstr(_vsFilename), "BasicVS", "vs_5_0", _vertexShaderBlob.GetAddressOf());
    CompileShader(StrToWstr(_psFilename), "BasicPS", "ps_5_0", _pixelShaderBlob.GetAddressOf());
}

ID3D10Blob *Shader::GetVertexShaderBlob() const
{
    return _vertexShaderBlob.Get();
}

ID3D10Blob *Shader::GetPixelShaderBlob() const
{
    return _pixelShaderBlob.Get();
}

void Shader::CompileShader(std::wstring filename, const char *entryPoint, const char *target, ID3DBlob **code)
{
    auto localResult = D3DCompileFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, target,
                                          D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, code, &_shaderErrorBlob);
    if (FAILED(localResult))
    {
        if (_shaderErrorBlob != nullptr)
        {
            GetErrorMessage(_shaderErrorBlob.Get());
        }
    }
}

void Shader::GetErrorMessage(ID3DBlob *errorBlob)
{
    std::string error;
    error.resize(errorBlob->GetBufferSize());
    std::copy_n((char *)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), error.begin());
    OutputDebugStringA(error.c_str());
}
