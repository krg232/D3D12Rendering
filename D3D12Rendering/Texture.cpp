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

void Texture::LoadTextures(std::vector<Mesh>& meshes)
{
	_textureDatas.resize(meshes.size());
    _result = CoInitializeEx(0, COINIT_MULTITHREADED);

    for (int i = 0; i < meshes.size(); i++)
    {
        std::filesystem::path ext = meshes.at(i).texPath;

        if (ext.extension() != ".tga")
        {
            _result = DirectX::LoadFromWICFile(StrToWstr(meshes.at(i).texPath).c_str(), DirectX::WIC_FLAGS_NONE, &(_textureDatas.at(i).texMetaData), _textureDatas.at(i).scratchImage);
        }
        else
        {
            _result = DirectX::LoadFromTGAFile(StrToWstr(meshes.at(i).texPath).c_str(),&(_textureDatas.at(i).texMetaData),_textureDatas.at(i).scratchImage);
        }
        
        auto img = _textureDatas.at(i).scratchImage.GetImage(0, 0, 0);
        _textureDatas.at(i).rawImage = *const_cast<DirectX::Image*>(img);
    }
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

const std::vector<TextureData>& Texture::GetTextureDatas() const
{
    return _textureDatas;
}
