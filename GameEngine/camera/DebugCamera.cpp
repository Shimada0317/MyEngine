#include "DebugCamera.h"
#include <cassert>

using namespace DirectX;

DebugCamera::DebugCamera(int window_width, int window_height)
	: Camera(window_width, window_height)
{
	// 画面サイズに対する相対的なスケールに調整
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void DebugCamera::RecalculationMatrix()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	//angleX = angX;
	//angleY = angY;



	//if (dirty || viewDirty) {
	//	// 追加回転分の回転行列を生成
	//	XMMATRIX matRotNew = XMMatrixIdentity();
	//	matRotNew *= XMMatrixRotationX(XMConvertToRadians(angleX));
	//	matRotNew *= XMMatrixRotationY(XMConvertToRadians(- angleY));
	//	// 累積の回転行列を合成
	//	// ※回転行列を累積していくと、誤差でスケーリングがかかる危険がある為
	//	// クォータニオンを使用する方が望ましい
	//	matRot = matRotNew * matRot;

	//	// 注視点から視点へのベクトルと、上方向ベクトル
	//	XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
	//	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };


	//	// ベクトルを回転
	//	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	//	vUp = XMVector3Transform(vUp, matRot);

	//	// 注視点からずらした位置に視点座標を決定
	//	XMFLOAT3 target = GetEye();

	//	//const XMFLOAT3& eye = GetEye();
	//	SetEye({target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	//	SetUp({ vUp.m128_f32[0], vUp.m128_f32[1],vUp.m128_f32[2] });
	//}

	Camera::RecalculationMatrix();
}
