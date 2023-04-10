#pragma once
#include <DirectXMath.h>
#include<memory>
#include"Audio.h"
#include"BaseScene.h"
#include"Camera.h"
#include "DirectXCommon.h"
#include"GameScene.h"
#include"Light.h"
#include"LightGroup.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"SceneManager.h"
#include"ParticleManager.h"
#include"PostEffect.h"
#include "Sprite.h"

using namespace std;

const int BUILAMOUNT = 16;

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
	using XMVECTOR = DirectX::XMVECTOR;
private:
	static const int debugTextNumber = 0;

public://メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="sceneManager_"></param>
	TitleScene(SceneManager* sceneManager_);

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
	/// Obj等のUpdateはこの中で
	/// </summary>
	void AllUpdate();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// カメラの移動
	/// </summary>
	void CameraDirection();

	/// <summary>
	/// カーソルが範囲内に入っているか
	/// </summary>
	/// <param name="cursorPos">カーソルの座標</param>
	/// <param name="checkPos">押せるスプライトの座標</param>
	/// <param name="radX">スプライトの横の範囲</param>
	/// <param name="radY">スプライトの縦の範囲</param>
	/// <param name="CheckFlag">範囲内に入っている場合フラグをtrueにする</param>
	void CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY,bool& CheckFlag);

	/// <summary>
	/// 矢印のスプライトの範囲
	/// </summary>
	/// <param name="cursorPos">カーソルの座標</param>
	/// <param name="checkPos">押せるスプライトの座標</param>
	/// <param name="radX">スプライトの横の範囲</param>
	/// <param name="radY">スプライトの縦の範囲</param>
	/// <returns></returns>
	bool NextorBack(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	/// <summary>
	/// 説明画面で行われる処理
	/// </summary>
	void DescriptionPageProces();

	/// <summary>
	///　フェードアウト後にゲームシーンへチェンジ
	/// </summary>
	void FadeOutAndSceneChange();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon)override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

private:
	//Obj
	unique_ptr <Object3d> Sphere;
	unique_ptr<Object3d> BillsHighAlpha[BUILAMOUNT];
	unique_ptr<Object3d> BillsLowAlpha[BUILAMOUNT];
	unique_ptr <Object3d> GroundObj;
	unique_ptr <Object3d >World;
	unique_ptr<Object3d> Start;
	//スプライト
	unique_ptr<Sprite> Title = nullptr;
	unique_ptr<Sprite> Cursor = nullptr;
	unique_ptr<Sprite> ClickBefore = nullptr;
	unique_ptr<Sprite> ClickAfter = nullptr;
	unique_ptr<Sprite> SignalBefore = nullptr;
	unique_ptr<Sprite> SignalAfter = nullptr;
	unique_ptr<Sprite> DescriptionOperation = nullptr;
	unique_ptr<Sprite> EnemyOverview = nullptr;
	unique_ptr<Sprite> GameStartPreparation = nullptr;
	unique_ptr<Sprite> ArrowRight = nullptr;
	unique_ptr<Sprite> ArrowLeft = nullptr;
	unique_ptr<Sprite> ArrowRightTrue = nullptr;
	unique_ptr<Sprite> ArrowLeftTrue = nullptr;
	//その他の機能
	unique_ptr<Light> light = nullptr;
	unique_ptr<LightGroup> lightGroupe = nullptr;
	unique_ptr<PostEffect> Post = nullptr;
	unique_ptr<Audio> ClickSe = nullptr;
	unique_ptr<Audio> MorseCodeSe = nullptr;
	unique_ptr<Audio> Bgm = nullptr;
	unique_ptr<Camera> TitleCamera = nullptr;
	//球体のステータス
	XMVECTOR SpherePos = { 0,0,0 };
	XMFLOAT3 SphereScl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 SphereRot = { 0,0,0 };
	//フィールドのステータス
	XMVECTOR GroundPos = { 22.5f,-1,40 };
	XMFLOAT3 GroundScl = { 13,20,16 };
	XMFLOAT3 GroundRot = { 0,0,0 };
	//ビル群のステータス
	XMFLOAT3 BillsScl = { 10.0f,10.0f,10.0f };
	XMVECTOR BillsHighAlphaPos = { 0.0f,0.0f,-16.5f };
	XMVECTOR BillsLowAlphaPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 BillsRot = { 0.0f,90.0f,0.0f };
	//カメラの移動先のステータス
	XMVECTOR StartPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 StartScl = { 15.0f,15.0f,15.0f };
	//カメラのステータス
	XMVECTOR CameraMove = { 0.0f,0.0f,0.0f };
	XMVECTOR CameraEyeMove = { 0.0f,0.0f,0.0f };
	bool CameraEyeMove_F = false;
	bool CameraChange_F = false;
	//足場のステータス
	XMVECTOR WorldPos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 WorldScl = { 100,100,100 };
	//スプライトのステータス
	XMFLOAT4 SpriteCol = { 1,1,1,1 };
	XMFLOAT2 Anchorpoint = { 0.5f,0.5f };
	XMFLOAT2 ReticlePos = { 640.0f,360.0f };
	XMFLOAT2 ClickPos = { WinApp::window_width/2,600 };
	//タイトルスプライトのステータス
	XMFLOAT2 TitleSize = { 1280.0f,720.0f };
	XMFLOAT2 TitlePos = { 0.0f,0.0f };
	bool TitleSprite_F = true;
	//説明の矢印座標
	XMFLOAT2 ArrowRightPos = { 1240.0f,300 };
	XMFLOAT2 ArrowLeftPos = { 35.0f,300 };
	XMFLOAT2 ArrowSize = { 32.f,32.f};
	XMFLOAT4 ArrowRightColor = { 1.f,1.f,1.f,1.f };
	XMFLOAT4 ArrowLeftColor = { 1.f,1.f,1.f,1.f };
	bool RightTrueIn_F = false;
	bool LeftTrueIn_F = false;
	//ポストエフェクトの色
	XMFLOAT4 PostEfectColor = { 0.0f,0.0f,0.0f,1.0f };
	//カーソルが入っているか
	bool SignalCurorInFlag = false;
	//フェードを掛けるか掛けないか
	bool FadeOut_F = false;
	bool TitleDisplay_F = true;
	//クリックしているかどうか
	bool Click_F = true;
	//音量
	float Volume = 0.8f;
	//説明ページ
	int DescriptionPage = 0;

	float PointLightPos[3] = { 0.f,0.f,0.f };
	float PointLightColor[3] = { 1.f,1.f,1.f };
	float PointLightAtten[3] = { 0.3f,0.1f,0.1f };


	XMFLOAT3 SpotLightDir = { 0,-1,0 };
	XMFLOAT3 SpotLightPos = { 0,1005,0 };
	XMFLOAT3 SpotLightColor = { 1,1,1 };
	XMFLOAT3 SpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 SpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 SpotLightDir2 = { 0,0,0 };
	XMFLOAT3 SpotLightPos2 = { 10,1005,160 };
	XMFLOAT3 SpotLightColor2 = { 0,0,1 };
	XMFLOAT3 SpotLightAtten2 = { 0.f,0.f,0.f };
	XMFLOAT2 SpotLightFactorAngle2 = { 20.0f,30.0f };

	float EasingTimer = 0.f;
	bool EasingChangeFlag=false;
	XMFLOAT2 ClickSize = { 550.f,60.f };
};

