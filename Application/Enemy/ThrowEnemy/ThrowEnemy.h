#pragma once
#include"Audio.h"
#include"Camera.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"ObjParticle.h"
#include"ParticleManager.h"
#include"Sprite.h"
#include<DirectXMath.h>

class ThrowEnemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	~ThrowEnemy();

	void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const bool& movement);

	void StatusSet();

	void AllUpdate();

	void Update();

	void Draw(DirectXCommon* dxCommon);

	void ChangeViewPort(XMMATRIX& matviewport);

	XMFLOAT2 WorldtoScreen(const XMVECTOR& set3Dposition);
private:
	//Obj
	//頭
	std::unique_ptr<Object3d> HeadPart;
	//両腕
	std::unique_ptr<Object3d> ArmsPart;
	//体
	std::unique_ptr<Object3d> BodyPart;
	//影
	std::unique_ptr<Object3d> Shadow;
	//中心
	std::unique_ptr<Object3d> Center;

	//Objパーティクル
	std::list<std::unique_ptr<ObjParticle>>Obj_Particle;

	//スプライト
	//体
	std::unique_ptr<Sprite> RockOn;
	//頭
	std::unique_ptr<Sprite> RockOnHead;

	//ダメージを食らったときのエフェクト
	ParticleManager* PartGreen = nullptr;
	ParticleManager* PartRed = nullptr;

	//カメラ
	Camera* BringUpCamera;

	//敵の中心部分のステータス
	XMVECTOR CenterWorldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 CenterRot = { 0.0f,0.0f,0.0f };
	XMMATRIX CenterMat;

	//敵が持っているステータス
	int Hp = 50;
	int OldHp = 0;
	bool RobotAriveFlag = false;
	XMVECTOR TrackPoint = { 0,0,0 };

	//パーツごとのスケール
	XMFLOAT3 HeadPartScl = { 0.3f,0.3f,0.3f };
	XMFLOAT3 BodyPartScl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 ArmsPartScl = { 0.2f,0.2f,0.2f };

	//パーツごとのポジション
	XMVECTOR HeadPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR BodyPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR ArmsPartPos = { 0.0f,0.0f,0.0f };

	//パーツごとの色
	XMFLOAT4 ArmsPartColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 HeadPartColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 BodyPartColor = { 1.0f,1.0f,1.0f,1.0f };

	//パーツごとの回転
	XMFLOAT3 ArmsPartRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 HeadPartRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 BodyPartRot = { 0.0f,0.0f,0.0f };

	//パーツごとに渡すステータス
	XMVECTOR AllPos = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 AllRot;

	//影のステータス
	XMVECTOR ShadowPos = { 0,0,0 };
	XMFLOAT4 ShadowCol = { 0.0f,0.0f,0.0f,0.1f };

	//敵が持つ2D系のステータス
	XMFLOAT2 RockOnPos = { 0.0f,0.0f };
	XMFLOAT2 RockOnAnchorPoint = { 0.5f,0.5f };
	XMFLOAT4 RockOnCol = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 RockOnHeadPos = { 0.0f,0.0f };

	//2D座標を持たせる計算で使う変数
	XMVECTOR offset;
	XMMATRIX MatViewPort;

	//変形用のフラグ
	bool DefomationFlag = false;
	float DefomationCount = 0.0f;
};

