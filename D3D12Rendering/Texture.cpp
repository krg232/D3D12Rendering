#include "Texture.h"

Texture::Texture()
{
}

void Texture::CreateTexSampler()
{
    _textureSamplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    _textureSamplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    _textureSamplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    _textureSamplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    _textureSamplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    _textureSamplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    _textureSamplerDesc.MinLOD = 0.0f;
    _textureSamplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    _textureSamplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
}

void Texture::LoadTexture(std::string path)
{
    _result = CoInitializeEx(0, COINIT_MULTITHREADED);
    _result = DirectX::LoadFromWICFile(StrToWstr(path).c_str(),
        DirectX::WIC_FLAGS_NONE, &_texMetaData, _scratchImage);
    auto img = _scratchImage.GetImage(0, 0, 0);
	_rawImage = const_cast<DirectX::Image*>(img);
}

DirectX::TexMetadata Texture::GetTexMetaData() const
{
	return _texMetaData;
}

DirectX::Image* Texture::GetRawImage() const
{
	return _rawImage;
}

D3D12_STATIC_SAMPLER_DESC Texture::GetTextureSamplerDesc() const
{
	return _textureSamplerDesc;
}
