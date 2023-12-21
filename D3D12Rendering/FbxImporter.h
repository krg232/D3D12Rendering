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
    std::vector<Mesh> &meshes;
    bool flipTextureU = false; // テクスチャU座標の反転
    bool flipTextureV = false; // テクスチャV座標の反転
};

class FbxImporter
{
public:
    void LoadFbx(ImportSettings settings); // fbxモデルの読み込み

private:
    void LoadMesh(Mesh &dst, const aiMesh *src, bool flipU, bool flipV);
    void LoadTexture(std::wstring filename, Mesh &dst, const aiMaterial *src); // テクスチャパスの取得
    Assimp::Importer _importer;
};
