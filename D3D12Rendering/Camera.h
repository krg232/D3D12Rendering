#pragma once

#include <DirectXMath.h>
#include "Struct.h"

class Camera
{
public:
	Camera();
	void InitCamera(DirectX::XMFLOAT3 eyePos, DirectX::XMFLOAT3 furcusPos, DirectX::XMFLOAT3 upDirection, int fov, int windowWidth, int windowHeight);
	DirectX::XMMATRIX GetCameraMatrix() const;

private:
	void CalcCameraMatrix();
	int _fov;
	int _windowWidth;
	int _windowHeight;
	ViewMatrix _viewMat; // ビュー行列xプロジェクション行列したもの
	DirectX::XMMATRIX _lookAt;
	DirectX::XMMATRIX _projMat;
	DirectX::XMMATRIX _cameraMat;
};
