#pragma once

#include <string>
#include <vector>
#include <memory>
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Struct.h"
#include "Utility.h"
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct ImportSettings
{
    std::wstring filePath = L"";
    std::vector<Mesh>& meshes;
    bool flipU = false; // Uç¿ïWîΩì]ÅH
    bool flipV = false; // Vç¿ïWîΩì]ÅH
};

class FbxImporter
{
public:
	void LoadFbx(ImportSettings settings);

private:
    void LoadMesh(Mesh& dst, const aiMesh* src, bool flipU, bool flipV);
    void LoadTexture(std::wstring filename, Mesh& dst, const aiMaterial* src);
    Assimp::Importer _importer;
};
