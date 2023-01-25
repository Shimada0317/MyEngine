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

	const XMFLOAT3& Getye() { return Eye; }

	const XMFLOAT3& GetTarget() { return Target; }

	const XMMATRIX& GetWorld() { return MatWorld; }

	void MatrixIdentity(const XMVECTOR& Pos, const XMFLOAT3& Rot);

	void Update(const XMVECTOR& vel,const XMFLOAT3& rot,Camera* NormalCam);

	void ShakeCamera(const XMVECTOR& Pos);

private:
	Camera* camera = nullptr;

	//std::unique_ptr<Object3d> debug;
	//ObjModel* debugModel;

	XMMATRIX World;

	// ワールド変換行列
	XMMATRIX MatWorld;

	XMMATRIX MatRot, MatTrans;

	XMVECTOR Position;
	XMFLOAT3 Rotation;

	// ビュー行列
	XMMATRIX MatView = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX MatProjection = DirectX::XMMatrixIdentity();
	// ビュー射影行列(ビュープロジェクション)
	XMMATRIX MatViewProjection = DirectX::XMMatrixIdentity();
	// 上方向ベクトル
	XMFLOAT3 Up = { 0, 1, 0 };
	//注視点　　
	XMFLOAT3 Eye = { 0.0f,0.0f,-5.0f };

	XMFLOAT3 Target = { 0.0f,0.0f,0.0f };

	// アスペクト比
	float AspectRatio = 1.0f;

	XMVECTOR EyePosition;
	// 注視点座標
	XMVECTOR TargetPosition;
	// （仮の）上方向
	XMVECTOR UpVector;
};