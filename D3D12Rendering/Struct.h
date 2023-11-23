#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::string texPath;
};

struct ViewMatrix
{
	DirectX::XMFLOAT3 eyePos;
	DirectX::XMFLOAT3 forcusPos;
	DirectX::XMFLOAT3 upDirection;
};