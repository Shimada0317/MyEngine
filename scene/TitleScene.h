#pragma once

#pragma once

#include"BaseScene.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"Input.h"
#include"ParticleManager.h"
#include"Player.h"
#include"Bullet.h"
#include"middle.h"
#include"PostEffect.h"
#include"Robot.h"
#include"PostEffect.h"
#include<memory>
#include"Light.h"
#include"Camera.h"

using namespace std;

const int BILL = 16;

class TitleScene :public BaseScene
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
	static const int debugTextNumber = 0;

public://メンバ変数
	TitleScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Updata() override;

	void CameraMove();

	void CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:

	ObjModel* billsModel = nullptr;
	ObjModel* ground = nullptr;
	ObjModel* spheremodel = nullptr;
	ObjModel* worldmodel = nullptr;
	ObjModel* startModel = nullptr;

	unique_ptr <Object3d> sphere;
	unique_ptr<Object3d> bills[BILL];
	unique_ptr<Object3d> bills1[BILL];
	unique_ptr <Object3d> groundObj;
	unique_ptr <Object3d >world;
	unique_ptr<Object3d> Start;

	Camera* camera = nullptr;

	XMVECTOR pos = { 0,0,0 };
	//フィールド
	XMVECTOR ground_pos = { 22.5f,-1,40 };
	XMFLOAT3 ground_scl = { 13,20,16 };
	XMFLOAT3 ground_rot = { 0,0,0 };
	//ビル群
	XMFLOAT3 billsScl = { 10.0f,10.0f,10.0f };
	XMVECTOR billsPos = { 0.0f,0.0f,-16.5f };
	XMVECTOR billsPos1 = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 billsRot = { 0.0f,90.0f,0.0f };

	XMVECTOR start_pos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 start_scl = { 15.0f,15.0f,15.0f };

	XMFLOAT3 camTar= { 0.0f,0.0f,0.0f };
	XMFLOAT3 camEye = { 0.0f,0.0f,0.0f };

	//足場
	XMVECTOR worldPos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 worldScl = { 100,100,100 };

	XMVECTOR camMove = { 0.0f,0.0f,0.0f };
	XMVECTOR camEyeMove = { 0.0f,0.0f,0.0f };

	//カーソル
	//スプライト
	XMFLOAT4 spCol = { 1,1,1,1 };
	XMFLOAT2 anc = { 0.5f,0.5f };
	XMFLOAT2 retpos = { 640.0f,360.0f };

	XMFLOAT2 ClickPos = { 380,560 };
	

	DirectXCommon* dxCommon = nullptr;

	Sprite* title = nullptr;
	Sprite* cursor = nullptr;
	Sprite* clickBefore = nullptr;
	Sprite* clickAfter = nullptr;
	Sprite* SignalBefore = nullptr;
	Sprite* SignalAfter = nullptr;
	Sprite* setumei = nullptr;

	Light* light = nullptr;

	PostEffect* post = nullptr;

	Player* pl = nullptr;

	XMFLOAT3 ramieru_pos = { 0,0,0 };

	XMFLOAT2 titlesize = { 1280.0f,720.0f };
	XMFLOAT2 titlepos = { 0.0f,0.0f };

	int timer = 0;

	XMFLOAT4 col = { 0.0f,0.0f,0.0f,1.0f };

	bool titleSprite = true;

	bool cameraEyeMove = false;
	bool cameraChange = false;

	bool cursorIn = false;
	bool start = true;
	bool Sognal = false;
};

