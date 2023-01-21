#pragma once

#pragma once

#include"Audio.h"
#include"BaseScene.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"Input.h"
#include"ParticleManager.h"
#include"PostEffect.h"
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
	using XMVECTOR = DirectX::XMVECTOR;
private:
	static const int debugTextNumber = 0;

public://メンバ変数
	TitleScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Update() override;

	void CameraMove();

	void CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	bool NextorBack(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:
	//Objモデル
	ObjModel* BillsModel = nullptr;
	ObjModel* Ground = nullptr;
	ObjModel* Spheremodel = nullptr;
	ObjModel* Worldmodel = nullptr;
	ObjModel* StartModel = nullptr;
	//Obj
	unique_ptr <Object3d> Sphere;
	unique_ptr<Object3d> Bills[BILL];
	unique_ptr<Object3d> Bills1[BILL];
	unique_ptr <Object3d> GroundObj;
	unique_ptr <Object3d >World;
	unique_ptr<Object3d> Start;
	//スプライト
	Sprite* title = nullptr;
	Sprite* cursor = nullptr;
	Sprite* clickBefore = nullptr;
	Sprite* clickAfter = nullptr;
	Sprite* SignalBefore = nullptr;
	Sprite* SignalAfter = nullptr;
	Sprite* setumei = nullptr;
	Sprite* setumei2 = nullptr;
	Sprite* setumei3 = nullptr;
	Sprite* arrowRight = nullptr;
	Sprite* arrowLeft = nullptr;
	Sprite* arrowRightTrue = nullptr;
	Sprite* arrowLeftTrue = nullptr;
	//その他の機能
	Light* light = nullptr;
	PostEffect* post = nullptr;
	Audio* Click_SE = nullptr;
	Audio* MorseCode_SE = nullptr;
	Audio* Bgm = nullptr;
	Camera* TitleCamera = nullptr;
	DirectXCommon* dxCommon = nullptr;
	//球体のステータス
	XMVECTOR Sphere_Pos = { 0,0,0 };
	XMFLOAT3 Sphere_Scl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 Sphere_Rot = { 0,0,0 };
	//フィールドのステータス
	XMVECTOR Ground_Pos = { 22.5f,-1,40 };
	XMFLOAT3 Ground_Scl = { 13,20,16 };
	XMFLOAT3 Ground_Rot = { 0,0,0 };
	//ビル群のステータス
	XMFLOAT3 Bills_Scl = { 10.0f,10.0f,10.0f };
	XMVECTOR Bills_Pos = { 0.0f,0.0f,-16.5f };
	XMVECTOR Bills_Pos1 = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 Bills_Rot = { 0.0f,90.0f,0.0f };
	//カメラの移動先のステータス
	XMVECTOR Start_Pos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 Start_Scl = { 15.0f,15.0f,15.0f };
	//カメラのステータス
	XMFLOAT3 Cam_Target= { 0.0f,0.0f,0.0f };
	XMFLOAT3 Cam_Eye = { 0.0f,0.0f,0.0f };
	XMVECTOR Cam_Move = { 0.0f,0.0f,0.0f };
	XMVECTOR CamEye_Move = { 0.0f,0.0f,0.0f };

	bool CameraEyeMove_F = false;
	bool CameraChange_F = false;
	//足場のステータス
	XMVECTOR World_Pos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 World_Scl = { 100,100,100 };
	//スプライトのステータス
	XMFLOAT4 Sprite_Col = { 1,1,1,1 };
	XMFLOAT2 Anchorpoint = { 0.5f,0.5f };
	XMFLOAT2 Reticle_Pos = { 640.0f,360.0f };
	XMFLOAT2 Click_Pos = { 380,560 };
	//タイトルスプライトのステータス
	XMFLOAT2 Title_Size = { 1280.0f,720.0f };
	XMFLOAT2 Title_Pos = { 0.0f,0.0f };
	
	bool TitleSprite_F = true;
	//説明の矢印座標
	XMFLOAT2 ArrowRight_Pos = { 1220.0f,300 };
	XMFLOAT2 ArrowLeft_Pos = { 30.0f,300 };
	
	bool RightTrueIn_F = false;
	bool LeftTrueIn_F = false;
	//ポストエフェクトの色
	XMFLOAT4 Post_Col = { 0.0f,0.0f,0.0f,1.0f };

	bool CursorIn_F = false;
	bool CantClick_F = true;
	bool FadeOut_F = false;
	bool TitleDisplay_F = true;
	bool Click_F = true;

	float CantClickTimer = 0.0f;
	float volume = 0.8f;

	int SetumeiPage = 0;

	
};

