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
#include"LightGroup.h"
#include"middle.h"
#include"ClearScene.h"

#include<memory>

using namespace std;

const int BILLS = 16;

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

	ObjModel* worldmodel = nullptr;
	ObjModel* startModel = nullptr;
	ObjModel* billsModel = nullptr;

	unique_ptr <Object3d> sphere;
	unique_ptr <Object3d> groundObj;
	unique_ptr <Object3d >world;
	unique_ptr<Object3d> Start;
	unique_ptr<Object3d> bills[BILLS];


	FbxModel* model = nullptr;
	FbxObject3d* Object = nullptr;

	Sprite* sprite = nullptr;
	Sprite* title = nullptr;



	XMFLOAT3 cameraEye = {0.0f,0.0f,-10};
	XMFLOAT3 cameraTarget = {0.0f,1.0f,0.0f};
	float cameradistance = 20.0f;

	XMVECTOR start_pos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 start_scl = { 15.0f,15.0f,15.0f };

	XMVECTOR pos = { 0,0,0 };
	//フィールド
	XMVECTOR ground_pos = { 22.5f,-1,40 };
	XMFLOAT3 ground_scl = { 13,20,16 };
	XMFLOAT3 ground_rot = { 0,0,0 };
	//足場
	XMVECTOR worldPos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 worldScl = { 100,100,100 };
	//ビル群
	XMFLOAT3 billsScl = { 10.0f,10.0f,10.0f };
	XMVECTOR billsPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 billsRot = { 0.0f,90.0f,0.0f };

	XMFLOAT2 screen_size = { 1280.0f,720.0f };

	PostEffect* postEffect = nullptr;

	bool JumpFlag = false;


	Light* light = nullptr;

	LightGroup* lightGroup = nullptr;

	bool change = false;

	unique_ptr <middle> mid = nullptr;

	int patern = 0;
	int playerHp;
	int oldHp;
	bool post = false;

	float clearT = 0;

	XMFLOAT4 col = { 0.0f,1.0f,1.0f,1.0f };

	float billsposY = { -150.0f };

};

