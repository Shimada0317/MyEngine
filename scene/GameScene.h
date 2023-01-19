#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include"Audio.h"
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

class GameScene :public BaseScene
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


public://メンバ関数

	GameScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void AllUpdata();

	void Update() override;

	void ObjDraw(DirectXCommon* dxCommon);

	void SpriteDraw(DirectXCommon* dxCommon);

	void ImgDraw();

	void PostEffectDraw(DirectXCommon* dxCommon);

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private://メンバ変数
	//Objモデル
	ObjModel* SphereModel = nullptr;
	ObjModel* WorldModel = nullptr;
	ObjModel* StartModel = nullptr;
	ObjModel* BillsModel = nullptr;
	//Obj
	unique_ptr <Object3d> Sphere;
	unique_ptr <Object3d> World;
	unique_ptr<Object3d> Start;
	unique_ptr<Object3d> Bills[BILLS];
	unique_ptr<Object3d> Bills1[BILLS];
	unique_ptr<Object3d> FieldBills[5];
	//スプライト
	Sprite* Clear = nullptr;
	Sprite* Conteniu = nullptr;
	//その他の機能
	Audio* Bgm = nullptr;
	PostEffect* postEffect = nullptr;
	Light* light = nullptr;
	unique_ptr <middle> Mid = nullptr;
	//最初のビルのステータス
	XMVECTOR Start_Pos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 Start_Scl = { 15.0f,15.0f,15.0f };
	XMFLOAT3 Start_Rot = { 0.0f,180.0f,0.0f };
	//天球のステータス
	XMVECTOR Sphere_Pos = { 0,0,0 };
	XMFLOAT3 Sphere_Scl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 Sphere_Rot = { 0,0,0 };
	//フィールドのステータス
	XMFLOAT3 FieldBill_Scl = { 6.0f,10.0f,7.0f };
	XMVECTOR FieldBill_pos[5];
	XMFLOAT3 FieldBill_Rot[5];
	//足場のステータス
	XMVECTOR World_Pos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 World_Scl = { 100,100,100 };
	//ビル群のステータス
	XMFLOAT3 Bills_Scl = { 10.0f,10.0f,10.0f };
	XMVECTOR Bills_Pos = { 0.0f,0.0f,-16.5f };
	XMVECTOR Bills_Pos1 = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 Bills_Rot = { 0.0f,90.0f,0.0f };
	//その他の変数
	XMFLOAT4 PostCol = { -1.0f,-1.0f,-1.0f,1.0f };
	int Patern = 0;
	int PlayerHp;
	int OldHp;
	bool PostEffectOn = false;
	bool ClearFlag = false;
	bool StopUpdate = false;
	bool DethFlag = false;
	bool GameStart = false;
};

