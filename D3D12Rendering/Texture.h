#pragma once

#include <string>
#include <memory>
#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include "Utility.h"

class Texture
{
public:
	Texture();
	void CreateTexSampler();
	void LoadTexture(std::string path);
	DirectX::TexMetadata GetTexMetaData() const;
	DirectX::Image* GetRawImage() const;
	D3D12_STATIC_SAMPLER_DESC GetTextureSamplerDesc() const;

private:
	HRESULT _result;

	D3D12_STATIC_SAMPLER_DESC _textureSamplerDesc = {};

	DirectX::Image* _rawImage;
	DirectX::TexMetadata _texMetaData = {};
	DirectX::ScratchImage _scratchImage = {};

};
