#pragma once
#include "BaseScene.h"
#include <DirectXMath.h>
#include"Robot.h"
#include"Body.h"
#include"Head.h"
#include"DebugCamera.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"ParticleManager.h"
#include"Player.h"
#include<memory>

class DebugScene :
    public BaseScene
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
private:
	static const int debugTextNumber = 0;

public://メンバ変数
	DebugScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Update() override;

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:
	std::unique_ptr<Robot>rob;
	std::unique_ptr<Body>bob;
	std::unique_ptr<Head>head;
	std::unique_ptr<Object3d> bo = nullptr;
	std::unique_ptr<Player>player;
	ObjModel* bomodel = nullptr;

	XMVECTOR position = { 0.0f,0.0f,0.1f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 0.1f,0.1f,0.1f };

	DebugCamera* camera = nullptr;

	XMVECTOR allpos = { 1.0f,1.0f,1.0f };
	XMFLOAT3 Eye_pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Target_pos = { 0.0f,0.0f,0.0f };
	bool arive = true;
};

