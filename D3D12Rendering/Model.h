#pragma once

#include <d3d12.h>
#include "Struct.h"

class Model
{
public:
	Model();
	void Load();
	Mesh GetMesh();

private:
	Mesh _mesh;
};
