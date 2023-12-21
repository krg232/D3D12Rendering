#pragma once

#include <string>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Utility.h"

class Shader
{
public:
	Shader();
	void Compile();
	ID3D10Blob *GetVertexShaderBlob() const;
	ID3D10Blob *GetPixelShaderBlob() const;

private:
	void CompileShader(std::wstring filename, const char *entryPoint, const char *target, ID3DBlob **code);
	void GetErrorMessage(ID3DBlob *errorBlob); // エラーメッセージを取得する
	Microsoft::WRL::ComPtr<ID3DBlob> _vertexShaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> _pixelShaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> _shaderErrorBlob;
	std::string _vsFilename;
	std::string _psFilename;
};
