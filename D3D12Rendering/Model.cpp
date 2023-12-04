#include "model.h"

Model::Model()
{
}

void Model::Load(std::string path)
{
	FbxImporter _importer;
	ImportSettings _settings = { StrToWstr(path),_meshes };

	_importer.LoadFbx(_settings);
}

Mesh Model::GetMesh()
{
	return this->_mesh;
}

std::vector<Mesh>& Model::GetMeshes()
{
	return _meshes;
}

