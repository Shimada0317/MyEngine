#include "Camera.h"

using namespace DirectX;

Camera::Camera(int window_width, int window_height)
{
	aspect = (float)window_width / window_height;
	//ビュー行列
	UpdatematView();
	//射影行列
	UpdatematProjection();
	//ビュープロジェクション
	matViewProjection = matView * matProjection;
}

void Camera::Iniitialize()
{
}

void Camera::Update()
{
	if (viewflag || projectionflag) {
		//ビュー行列の再計算があるなら
		if (viewflag) {
			//再計算(更新)
			UpdatematView();
			viewflag = false;
		}
		//射影行列の再計算があるなら
		if (projectionflag) {
			//再計算(更新)
			UpdatematProjection();
			projectionflag = false;
		}
		//ビュープロジェクション
		matViewProjection = matView * matProjection;
	}

}

void Camera::UpdatematView()
{
	//視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	//注視点座標
	XMVECTOR targetPos = XMLoadFloat3(&target);
	//上方向
	XMVECTOR upVecror = XMLoadFloat3(&up);

	//カメラZ軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePosition);
	//0ベクトルだと向きが定まらないので
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVecror, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVecror));
	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	//カメラのX軸
	XMVECTOR cameraAxisX;
	//X軸は上方向→X軸の外積
	cameraAxisX = XMVector3Cross(upVecror, cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸
	XMVECTOR cameraAxisY;
	//Y軸
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//カメラ回転
	XMMATRIX matCameraRot;
	//カメラ座標からワールド座標
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	//転置により逆行列計算
	matView = XMMatrixTranspose(matCameraRot);
	//視点座標に-1を掛けた計算
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル
	XMVECTOR tx = XMVector3Dot(cameraAxisX, reverseEyePosition);//X
	XMVECTOR ty = XMVector3Dot(cameraAxisY, reverseEyePosition);//Y
	XMVECTOR tz = XMVector3Dot(cameraAxisZ, reverseEyePosition);//Z
	//1つにまとめる
	XMVECTOR translation = XMVectorSet(tx.m128_f32[0], ty.m128_f32[1], tz.m128_f32[2], 1.0f);
	//ビュー行列に平行移動
	matView.r[3] = translation;

	//ビルボード行列
	matBillbord.r[0] = cameraAxisX;
	matBillbord.r[1] = cameraAxisY;
	matBillbord.r[2] = cameraAxisZ;
	matBillbord.r[3] = XMVectorSet(0, 0, 0, 1);

	//カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVecror);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y軸回りビルボード行列
	matBillbordY.r[0] = ybillCameraAxisX;
	matBillbordY.r[1] = ybillCameraAxisY;
	matBillbordY.r[2] = ybillCameraAxisZ;
	matBillbordY.r[3] = XMVectorSet(0, 0, 0, 1);

}

void Camera::UpdatematProjection()
{
	//透視投影による射影行列
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspect,
		0.1f, 1000.0f
	);
}

void Camera::MoveEyeVectror(const XMFLOAT3& move)
{
	//視点座標を移動し、反映
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Camera::MoveEyeVector(const XMVECTOR& move)
{
	//視点座標を移動し、反映
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
}

void Camera::MoveVector(const XMFLOAT3& move)
{
	//視点と注視点座標を移動し、反映
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::MoveVector(const XMVECTOR& move)
{
	//視点と注視点座標を移動し、反映
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	target_moved.x += move.m128_f32[0];
	target_moved.y += move.m128_f32[1];
	target_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
	SetTarget(target_moved);
}