#pragma once

#include <DirectXMath.h>
#include "Struct.h"

class Camera
{
public:
	Camera();
	void InitCamera(DirectX::XMFLOAT3 eyePos, DirectX::XMFLOAT3 focusPos, DirectX::XMFLOAT3 upDirection, int fov, int windowWidth, int windowHeight);
	DirectX::XMMATRIX GetCameraMatrix() const;
	void RotateCamera(float yawDelta, float pitchDelta);

private:
	void CalcCameraMatrix();
	int _fov;
	int _windowWidth;
	int _windowHeight;
	ViewMatrix _viewMat;
	DirectX::XMMATRIX _lookAt;
	DirectX::XMMATRIX _projMat;
	DirectX::XMMATRIX _cameraMat; // ビュー行列xプロジェクション行列したもの

	const float _rotationSensitivity = 0.005f; // マウス移動量に対する回転角度の係数
};
