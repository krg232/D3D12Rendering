#include "camera.h"

Camera::Camera()
{
}

void Camera::InitCamera(DirectX::XMFLOAT3 eyePos, DirectX::XMFLOAT3 furcusPos, DirectX::XMFLOAT3 upDirection, int fov, int windowWidth, int windowHeight)
{
	// カメラ初期化
	_fov = fov; // 注：中心からの角度の倍
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_viewMat.eyePos = eyePos;
	_viewMat.forcusPos = furcusPos;
	_viewMat.upDirection = upDirection;
	CalcCameraMatrix();
}

DirectX::XMMATRIX Camera::GetCameraMatrix() const
{
	return _cameraMat;
}

void Camera::CalcCameraMatrix()
{
	// ビュー行列の計算
	_lookAt = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&_viewMat.eyePos),
		DirectX::XMLoadFloat3(&_viewMat.forcusPos),
		DirectX::XMLoadFloat3(&_viewMat.upDirection));
	// プロジェクション行列の計算
	_projMat = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(_fov),
		static_cast<float>(_windowWidth) / static_cast<float>(_windowHeight),
		0.3f,  // ニアクリップ
		1000.f // ファークリップ
	);
	_cameraMat = _lookAt * _projMat;
}
