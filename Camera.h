#pragma once
#include<DirectXMath.h>

class Camera
{
private://エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://メンバ関数

	Camera(int window_width, int window_height);

	void Iniitialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

/// <summary>
/// ビュー行列の更新
/// </summary>
	void UpdatematView();

/// <summary>
/// 射影行列の更新
/// </summary>
	void UpdatematProjection();

private:
	//ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	//ビルボード
	XMMATRIX matBillbord = DirectX::XMMatrixIdentity();
	//射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	//視点座標
	XMFLOAT3 eye = { 0,0,-20 };
	//注視点座標
	XMFLOAT3 target = { 0,1,0 };
	//上方向ベクトル
	XMFLOAT3 up = { 0,1,0 };

};

