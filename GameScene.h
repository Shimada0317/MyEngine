#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"Input.h"
#include"BaseScene.h"
#include"FbxObject3d.h"
#include"DebugCamera.h"
#include"PostEffect.h"

class GameScene:public BaseScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	

public://メンバ変数

	GameScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Update() override;

	void ObjDraw(DirectXCommon* dxCommon);

	void SpriteDraw(DirectXCommon* dxCommon);

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:

	DirectXCommon* dxCommon = nullptr;

	ObjModel* playermodel = nullptr;
	ObjModel* ramieru = nullptr;
	ObjModel* human = nullptr;

	Object3d* player3d = nullptr;
	Object3d* ramieru3d = nullptr;
	Object3d* human3d = nullptr;

	WinApp* winApp = nullptr;

	FbxModel* model = nullptr;
	FbxObject3d* Object = nullptr;

	Sprite* sprite = nullptr;
	Sprite* title = nullptr;

	DebugCamera* camera = nullptr;

	XMFLOAT3 cameraEye = {0,0,90};
	XMFLOAT3 cameraTarget = {0,2.5f,0};
	float cameradistance = 8.0f;

	XMFLOAT3 ramieru_pos = { 0,0,0 };
	XMFLOAT3 ramieru_rot = { 0,0,0 };
	XMFLOAT3 ramieru_scl = { 1.0f,1.0f,1.0f };

	XMFLOAT3 obj_rot = { 0,90,0 };

	XMFLOAT2 screen_size = { 1280.0f,720.0f };

	PostEffect* postEffect = nullptr;
};

