#pragma once
#include "BaseScene.h"
#include <DirectXMath.h>
#include"Enemy.h"
#include"DebugCamera.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"ObjParticle.h"
#include"ParticleManager.h"
#include"Player.h"
#include"Light.h"
#include"Mouse.h"
#include"Texture.h"
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
	Texture* tex = nullptr;
	XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	XMFLOAT3 size = { 5.0f,5.0f,10.0f };
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

	XMFLOAT3 cameraEye = { 0.0f,0.0f,-20.0f };
	XMFLOAT3 cameraTarget = { 0.0f,1.0f,0.0f };

};


