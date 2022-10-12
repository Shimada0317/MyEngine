#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include"camera/Camera.h"

#include"Object3d.h"
#include"ObjModel.h"
#include<memory>

class RailCamera
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:

	~RailCamera();

	const XMFLOAT3& Getye() { return eye; }

	const XMFLOAT3& GetTarget() { return target; }

	const XMMATRIX& GetWorld() { return matWorld; }

	void Initialize(const XMVECTOR& Pos, const XMFLOAT3& Rot);

	void UpdateWorld();

	void Update(const XMVECTOR& vel={0.0f,0.0f,0.0f},const XMFLOAT3& rot={0.0f,0.0f,0.0f});

	void Draw();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

private:
	Camera* camera = nullptr;

	std::unique_ptr<Object3d> debug;
	ObjModel* debugModel;

	XMMATRIX world;

	// ワールド変換行列
	XMMATRIX matWorld;

	XMMATRIX matRot, matTrans;

	XMVECTOR position;
	XMFLOAT3 rotation;

	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// ビュー射影行列(ビュープロジェクション)
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	//注視点　　
	XMFLOAT3 eye = { 0.0f,0.0f,0.0f };

	XMFLOAT3 target = { 0.0f,0.0f,0.0f };

	// アスペクト比
	float aspectRatio = 1.0f;

	XMVECTOR eyePosition;
	// 注視点座標
	XMVECTOR targetPosition;
	// （仮の）上方向
	XMVECTOR upVector;

};

