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
	_viewMat.focusPos = furcusPos;
	_viewMat.upDirection = upDirection;
	CalcCameraMatrix();
}

DirectX::XMMATRIX Camera::GetCameraMatrix() const
{
	return _cameraMat;
}

void Camera::RotateCamera(float yawDelta, float pitchDelta)
{
	// 感度を適用
	yawDelta *= _rotationSensitivity;
	pitchDelta *= _rotationSensitivity;

	DirectX::XMVECTOR eyePosition = DirectX::XMLoadFloat3(&_viewMat.eyePos);
	DirectX::XMVECTOR focusPosition = DirectX::XMLoadFloat3(&_viewMat.focusPos);
	DirectX::XMVECTOR upDirection = DirectX::XMLoadFloat3(&_viewMat.upDirection);

	// 注視点から視点へのベクトル
	DirectX::XMVECTOR eyeToFocusVec = DirectX::XMVectorSubtract(focusPosition, eyePosition);

	// ヨー回転（Y軸）
	DirectX::XMMATRIX yawRotation = DirectX::XMMatrixRotationY(yawDelta);
	eyeToFocusVec = DirectX::XMVector3TransformNormal(eyeToFocusVec, yawRotation);
	upDirection = DirectX::XMVector3TransformNormal(upDirection, yawRotation);

	// ピッチ回転（X軸）
	// 右方向ベクトルを計算 (Upと視線方向の外積)
	DirectX::XMVECTOR rightDirection = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upDirection, eyeToFocusVec));
	// Y軸とのなす角からピッチ角を求める
	auto currentPitch = asinf(DirectX::XMVectorGetY(DirectX::XMVector3Normalize(DirectX::XMVectorNegate(eyeToFocusVec))));
	auto newPitch = currentPitch + pitchDelta;
	// 実際に適用するピッチ回転量を計算
	auto actualPitchDelta = newPitch - currentPitch;

	DirectX::XMMATRIX pitchRotation = DirectX::XMMatrixRotationAxis(rightDirection, actualPitchDelta);
	// 視点ベクトルとUpベクトルをピッチ回転
	eyeToFocusVec = DirectX::XMVector3TransformNormal(eyeToFocusVec, pitchRotation);
	upDirection = DirectX::XMVector3TransformNormal(upDirection, pitchRotation);

	// 新しい視点位置を計算
	DirectX::XMVECTOR newEyePosition = DirectX::XMVectorSubtract(focusPosition, eyeToFocusVec);

	// Upベクトルを正規化
	upDirection = DirectX::XMVector3Normalize(upDirection);

	// 結果をメンバ変数に格納
	DirectX::XMStoreFloat3(&_viewMat.eyePos, newEyePosition);
	DirectX::XMStoreFloat3(&_viewMat.upDirection, upDirection);

	// カメラ行列を更新
	CalcCameraMatrix();
}

void Camera::CalcCameraMatrix()
{
	// ビュー行列の計算
	_lookAt = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&_viewMat.eyePos),
		DirectX::XMLoadFloat3(&_viewMat.focusPos),
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
