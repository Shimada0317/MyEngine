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
#include"ParticleManager.h"
#include"Light.h"
#include"middle.h"
#include"ClearScene.h"

#include<memory>

using namespace std;

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

	void AllUpdata();

	void Updata() override;

	void ObjDraw(DirectXCommon* dxCommon);

	void SpriteDraw(DirectXCommon* dxCommon);

	void ImgDraw();

	void PostEffectDraw(DirectXCommon* dxCommon);

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:

	DirectXCommon* dxCommon = nullptr;

	ObjModel* playermodel = nullptr;
	ObjModel* ground = nullptr;
	ObjModel* p = nullptr;

	unique_ptr <Object3d> sphere;
	unique_ptr <Object3d> groundObj;

	FbxModel* model = nullptr;
	FbxObject3d* Object = nullptr;

	Sprite* sprite = nullptr;
	Sprite* title = nullptr;

	DebugCamera* camera = nullptr;


	XMFLOAT3 cameraEye = {0.0f,0.0f,-10};
	XMFLOAT3 cameraTarget = {0.0f,1.0f,0.0f};
	float cameradistance = 20.0f;



	XMVECTOR pos = { 0,0,0 };

	XMVECTOR ground_pos = { 0,-1,5 };
	XMFLOAT3 ground_scl = { 5,5,50 };
	XMFLOAT3 ground_rot = { 0,90,0 };



	XMFLOAT2 screen_size = { 1280.0f,720.0f };

	PostEffect* postEffect = nullptr;

	bool JumpFlag = false;


	Light* light = nullptr;

	bool change = false;

	unique_ptr <middle> mid = nullptr;

	int patern = 0;
	int playerHp;
};

