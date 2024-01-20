#include "model.h"

Model::Model()
{
}

void Model::LoadModel(std::string path)
{
	FbxImporter _importer;
	// テクスチャのV座標だけ反転させる
	ImportSettings _settings = {StrToWstr(path), _meshes, false, true};

	_importer.LoadFbx(_settings);
}

std::vector<Mesh> &Model::GetMeshes()
{
	return _meshes;
}
