#include "model.h"

Model::Model()
{
}

void Model::Load()
{
	_mesh.vertices =
	{
		{{-1.f,-1.f,0.0f},{0.0f,1.0f}},
		{{-1.f,1.f,0.0f} ,{0.0f,0.0f}},
		{{1.f,-1.f,0.0f} ,{1.0f,1.0f}},
		{{1.f,1.f,0.0f} ,{1.0f,0.0f}},
	};
	_mesh.indices = { 0,1,2,2,1,3 };
}

Mesh Model::GetMesh()
{
	return this->_mesh;
}
