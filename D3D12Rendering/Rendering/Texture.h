#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include "..\Utility.h"
#include "Struct.h"

class Texture
{
public:
	Texture();
	void InitTexSampler();
	void LoadTextures(std::vector<Mesh> &meshes);
	D3D12_STATIC_SAMPLER_DESC GetTextureSamplerDesc() const;
	const std::vector<TextureData> &GetTextureDatas() const;

private:
	HRESULT _result;

	D3D12_STATIC_SAMPLER_DESC _textureSamplerDesc = {};

	DirectX::Image *_rawImage;
	DirectX::TexMetadata _texMetaData = {};
	DirectX::ScratchImage _scratchImage = {};

	std::vector<TextureData> _textureDatas;
};
