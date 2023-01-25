#pragma once
#include "Camera.h"
#include "Input.h"


/// <summary>
/// デバッグ用カメラ
/// </summary>
class DebugCamera :
	public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <param name="input">入力</param>
	DebugCamera(int window_width, int window_height);

	void Patern(const int patern,bool chan );

	void SetVel(const XMVECTOR vel);

	void Angle(const XMFLOAT3 ang);

	// 更新
	void RecalculationMatrix() override;

	void Draw();

	void SetDistance(float distance) {
		this->distance = distance; viewDirty = true;
	}

private:
	// 入力クラスのポインタ

	// カメラ注視点までの距離
	float distance = 20;
	// スケーリング
	float scaleX = 1.0f;
	float scaleY = 1.0f;
	// 回転行列
	float angX = 0;
	float angY = 0;

	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	XMVECTOR velo;
};

