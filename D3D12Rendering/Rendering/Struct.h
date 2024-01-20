#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>
#include <DirectXTex.h>

// 頂点情報の構造体
struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
};

// メッシュ情報の構造体
struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::string texPath;
};

// テクスチャ情報の構造体
struct TextureData
{
	DirectX::Image rawImage;
	DirectX::TexMetadata texMetaData;
	DirectX::ScratchImage scratchImage;
};

// ビュー行列の構造体
struct ViewMatrix
{
	DirectX::XMFLOAT3 eyePos;
	DirectX::XMFLOAT3 focusPos;
	DirectX::XMFLOAT3 upDirection;
};