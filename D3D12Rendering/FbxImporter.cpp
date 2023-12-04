#include "FbxImporter.h"

std::wstring GetDirectoryPath(const std::wstring& filePath)
{
	std::filesystem::path path = filePath.c_str();
	return path.remove_filename().c_str();
}

void FbxImporter::LoadFbx(ImportSettings settings)
{
    if (settings.filePath == L"")
    {
        // エラー処理
    }
    auto path = WstrToStr(settings.filePath);

    int flag = 0;
    flag |= aiProcess_Triangulate
        | aiProcess_PreTransformVertices
        | aiProcess_CalcTangentSpace
        | aiProcess_GenSmoothNormals
        | aiProcess_GenUVCoords
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_OptimizeMeshes;

    auto scene = _importer.ReadFile(path, flag);

    if (scene == nullptr)
    {
        // エラー処理
    }

    std::vector<Mesh>& meshes = settings.meshes;
    meshes.clear();
    meshes.resize(scene->mNumMeshes);

    for (size_t i = 0; i < meshes.size(); ++i)
    {
        const auto pMesh = scene->mMeshes[i];
        LoadMesh(meshes[i], pMesh);
        const auto pMaterial = scene->mMaterials[i];
        LoadTexture(settings.filePath, meshes[i], pMaterial);
    }

    scene = nullptr;
}

void FbxImporter::LoadMesh(Mesh& dst, const aiMesh* src)
{
    aiVector3D zeroVec3D(0.0f, 0.0f, 0.0f);
    aiColor4D zeroColor(0.0f, 0.0f, 0.0f, 0.0f);

    dst.vertices.resize(src->mNumVertices);

    for (auto i = 0u; i < src->mNumVertices; ++i)
    {
        auto position = &(src->mVertices[i]);
        auto normal = &(src->mNormals[i]);
        auto uv = src->HasTextureCoords(0) ? &(src->mTextureCoords[0][i]) : &zeroVec3D;
        auto tangent = src->HasTangentsAndBitangents() ? &(src->mTangents[i]) : &zeroVec3D;

        Vertex vertex = {};
        vertex.pos = DirectX::XMFLOAT3(position->x, position->y, position->z);
        vertex.normal = DirectX::XMFLOAT3(normal->x, normal->y, normal->z);
        vertex.uv = DirectX::XMFLOAT2(uv->x, uv->y);
        vertex.tangent = DirectX::XMFLOAT3(tangent->x, tangent->y, tangent->z);

        dst.vertices.at(i) = vertex;
    }

    dst.indices.resize(src->mNumFaces * 3);

    for (auto i = 0; i < src->mNumFaces; ++i)
    {
        const auto& face = src->mFaces[i];

        dst.indices.at(i * 3 + 0) = face.mIndices[0];
        dst.indices.at(i * 3 + 1) = face.mIndices[1];
        dst.indices.at(i * 3 + 2) = face.mIndices[2];
    }
}

void FbxImporter::LoadTexture(std::wstring filePath, Mesh& dst, const aiMaterial* src)
{
    aiString path;
    if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
    {
		// 相対パスを絶対パスに変換
        auto dir = GetDirectoryPath(filePath);
        auto file = std::string(path.C_Str());
        dst.texPath = WstrToStr(dir) + file;
    }
    else
    {
        dst.texPath.clear();
    }
}
