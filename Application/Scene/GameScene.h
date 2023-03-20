#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include"Audio.h"
#include "Sprite.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"BaseScene.h"
#include"DebugCamera.h"
#include"PostEffect.h"
#include"ParticleManager.h"
#include"Light.h"
#include"Actor.h"

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
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="sceneManager_"></param>
	GameScene(SceneManager* sceneManager_);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="dxComon"></param>
	void Initialize(DirectXCommon* dxComon) override;

	/// <summary>
	/// ステータスセット
	/// </summary>
	void StatusSet();

	/// <summary>
	/// オブジェクト等の全ての更新処理
	/// </summary>
	void AllUpdata();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// オブジェクトの描画
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void ObjDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// スプライトの描画
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void SpriteDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// ImgUI描画
	/// </summary>
	void ImgDraw();

	/// <summary>
	/// ポストエフェクトの描画
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void PostEffectDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon">dxCommonからコマンドリストをゲット</param>
	void Draw(DirectXCommon* dxCommon)override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	void SpotLightMove();
private://メンバ変数
	//Obj
	unique_ptr <Object3d> Sphere;
	unique_ptr <Object3d> World;
	unique_ptr<Object3d> Start;
	unique_ptr<Object3d> BillsHighAlpha[BILLS];
	unique_ptr<Object3d> BillsLowAlpha[BILLS];
	unique_ptr<Object3d> FieldBills[5];
	//スプライト
	unique_ptr<Sprite> Clear = nullptr;
	unique_ptr<Sprite> Conteniu = nullptr;
	unique_ptr<Sprite> DamageEfectSp = nullptr;
	//その他の機能
	Audio* Bgm = nullptr;
	PostEffect* postEffect = nullptr;
	Light* light = nullptr;
	LightGroup* lightGroupe = nullptr;
	unique_ptr <Actor> Act = nullptr;
	//最初のビルのステータス
	XMVECTOR StartPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 StartScl = { 15.0f,15.0f,15.0f };
	XMFLOAT3 StartRot = { 0.0f,180.0f,0.0f };
	//天球のステータス
	XMVECTOR SpherePos = { 0,0,0 };
	XMFLOAT3 SphereScl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 SphereRot = { 0,90,0 };
	//フィールドのステータス
	XMFLOAT3 FieldBillScl = { 6.0f,10.0f,7.0f };
	XMVECTOR FieldBillPos[5];
	XMFLOAT3 FieldBillRot[5];
	//足場のステータス
	XMVECTOR WorldPos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 WorldScl = { 100,100,100 };
	//ビル群のステータス
	XMFLOAT3 BillsScl = { 10.0f,10.0f,10.0f };
	XMVECTOR BillsHighAlphaPos = { 0.0f,0.0f,-16.5f };
	XMVECTOR BillsLowAlphaPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 BillsRot = { 0.0f,90.0f,0.0f };
	//その他の変数
	XMFLOAT4 PostCol = { -1.0f,-1.0f,-1.0f,1.0f };
	int Patern = 0;
	int PlayerHp;
	int OldHp;
	bool PostEffectOnFlag = false;
	bool ClearFlag = false;
	bool StopUpdateFlag = false;
	bool DethFlag = false;
	bool GameStartFlag = false;
	bool DamageHitFlag = false;
	XMFLOAT4 DamageEfectColor = { 1.f,1.f,1.f,1.f };


	XMVECTOR lightDir = { 0,0,0,0 };

	XMFLOAT3 FieldSpotLightDir = { 0,-10,0 };
	XMFLOAT3 FieldSpotLightPos = { 0,505,50 };
	XMFLOAT3 FieldSpotLightColor = { 0.9,0.5f,0.f };
	XMFLOAT3 FieldSpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 FieldSpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 PlayerSpotLightDir = { 0,-20,0 };
	XMFLOAT3 PlayerSpotLightPos = { 0,35,0 };
	XMFLOAT3 PlayerSpotLightColor = { 1.f,1.f,1.f };
	XMFLOAT3 PlayerSpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 PlayerSpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 SearchLightDir[3];
	XMFLOAT3 SearchLightPos[3];
	XMFLOAT3 SearchLightColor = { 1,1,1 };
	XMFLOAT3 SearchLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 SearchLightFactorAngle = { 20.0f,30.0f };
	
	bool SpotLightPositionChange=false;
	bool LightPositionChangeX[2];
	bool LightPositionChangeZ[2];

	float duration = 1.f;
	float time = -1.0f;
	float value = 0.f;
	float StartPointZ = 60.f;
	float EndPointZ = -20.f;
	float StartPointX = -10;
	float EndPointX = 10;

	bool easing = true;
	float EasingWaitTimeR = 0.f;
	bool EasingChange = false;
};

