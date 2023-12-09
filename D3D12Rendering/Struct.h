#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::string texPath;
};

struct TextureData
{
	DirectX::Image rawImage;
	DirectX::TexMetadata texMetaData;
	DirectX::ScratchImage scratchImage;
};

struct ViewMatrix
{
	DirectX::XMFLOAT3 eyePos;
	DirectX::XMFLOAT3 forcusPos;
	DirectX::XMFLOAT3 upDirection;
};