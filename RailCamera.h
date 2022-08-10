#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include"camera/Camera.h"
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

public:
	RailCamera(int window_width, int window_height);

	~RailCamera();

	void Initialize();

	void Update();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

private:
	Camera* camera = nullptr;

	XMMATRIX world;

	// ローカルワールド変換行列
	XMMATRIX matWorld;

	XMMATRIX matRot, matTrans;

	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 eye = { 0,0,0 };

	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// ビュー射影行列
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	// アスペクト比
	float aspectRatio = 1.0f;
};

