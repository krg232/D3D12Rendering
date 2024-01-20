#pragma once

#include <vector>
#include <string>
#include <d3d12.h>
#include "..\Rendering\Struct.h"
#include "FbxImporter.h"

class Model
{
public:
	Model();
	void LoadModel(std::string path);
	std::vector<Mesh>& GetMeshes();

private:
	std::vector<Mesh> _meshes;
};
