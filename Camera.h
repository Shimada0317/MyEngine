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

	virtual ~Camera();

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

/// <summary>
/// ビュー行列
/// </summary>
	inline const XMMATRIX& GetViewMatrix() {
		return matView;
	}

/// <summary>
/// 射影行列
/// </summary>
	inline const XMMATRIX& GetProjectionMatrix() {
		return matProjection;
	}


	/// <summary>
	/// ビルボード行列
	/// </summary>
	inline const XMMATRIX& GetBilbordMatrix() {
		return matBillbord;
	}
/// <summary>
/// 視点座標
/// </summary>
	inline const XMFLOAT3& GetEye() {
		return eye;
	}

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <param name="eye"></param>
	inline void SetEye(XMFLOAT3 eye) {
		this->eye = eye; viewflag = true;
	}

	/// <summary>
	/// 注視点座標
	/// </summary>
	/// <returns></returns>
	inline const XMFLOAT3& GetTarget() {
		return target;
	}

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <param name="target"></param>
	inline void SetTarget(XMFLOAT3 target) {
		this->target = target; viewflag = true;
	}

	/// <summary>
	/// 上方向のベクトル
	/// </summary>
	/// <returns></returns>
	inline const XMFLOAT3& GetUp() {
		return up;
	}

	/// <summary>
	/// 上方向のベクトル取得
	/// </summary>
	/// <param name="up"></param>
	inline void SetUp(XMFLOAT3 up) {
		this->up = up;
	}
	
	void MoveEyeVectror(const XMFLOAT3& move);
	void MoveEyeVector(const XMVECTOR& move);


	void MoveVector(const XMFLOAT3& move);
	void MoveVector(const XMVECTOR& move);

private:
	//ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	//射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	//ビルボード
	XMMATRIX matBillbord = DirectX::XMMatrixIdentity();
	//Y軸回りのビルボード行列
	XMMATRIX matBillbordY = DirectX::XMMatrixIdentity();
	//ビューと射影の合成
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	//ビュー行列のフラグ
	bool viewflag = false;
	//射影行列のフラグ
	bool projectionflag = false;
	//視点座標
	XMFLOAT3 eye = { 0,0,-20 };
	//注視点座標
	XMFLOAT3 target = { 0,0,0 };
	//上方向ベクトル
	XMFLOAT3 up = { 0,1,0 };
	//アス比
	float aspect = 1.0f;

};

