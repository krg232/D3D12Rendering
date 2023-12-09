#include "model.h"

Model::Model()
{
}

void Model::Load(std::string path)
{
	FbxImporter _importer;
	ImportSettings _settings = { StrToWstr(path),_meshes ,false,true};

	_importer.LoadFbx(_settings);
}

std::vector<Mesh>& Model::GetMeshes()
{
	return _meshes;
}

