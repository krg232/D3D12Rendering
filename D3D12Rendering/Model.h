#pragma once

#include <vector>
#include <string>
#include <d3d12.h>
#include "Struct.h"
#include "FbxImporter.h"

class Model
{
public:
	Model();
	void Load(std::string path);
	Mesh GetMesh();
	std::vector<Mesh>& GetMeshes();

private:
	Mesh _mesh;
	std::vector<Mesh> _meshes;
};
