#pragma once
#include"Camera.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"ParticleManager.h"
#include"Sprite.h"
#include<DirectXMath.h>
#include<list>

class ThrowEnemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:

	//デストラクタ
	~ThrowEnemy();

	/// <summary>
	/// 初期か
	/// </summary>
	/// <param name="allrot">共通の角度</param>
	/// <param name="allpos">共通の座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="trackpos">追従先の座標</param>
	void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos);

	void StatusSet();

	void AllUpdate();

	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="playerHp">プレイヤーのHP</param>
	void Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);

public:
	//オブジェクト
	std::unique_ptr<Object3d>HeadPart;
	std::unique_ptr<Object3d>BodyPart;
	std::unique_ptr<Object3d>ArmsPart;
	std::unique_ptr<Object3d>Shadow;
	std::unique_ptr<Object3d>Center;

	//スプライト
	std::unique_ptr<Sprite> RockOn;
	//カメラ
	Camera* HaveCamera = nullptr;

	//パーツごとのステータス
	//敵の中心部分
	XMVECTOR CenterWorldPos = { 0.f,0.f,0.f };
	XMVECTOR AllPos = { 0.f,0.f,0.f };
	XMFLOAT3 CenterRot = { 0.f,0.f,0.f };
	XMMATRIX CenterMat;
	//頭
	XMVECTOR HeadPos = { 0.f,0.f,0.f };
	XMFLOAT3 HeadRot = { 0.f,0.f,0.f };
	XMFLOAT3 HeadScl = { 0.f,0.f,0.f };
	//体
	XMVECTOR BodyPos = { 0.f,0.f,0.f };
	XMFLOAT3 BodyRot = { 0.f,0.f,0.f };
	XMFLOAT3 BodyScl = { 0.f,0.f,0.f };
	//腕
	XMVECTOR ArmsPos = { 0.f,0.f,0.f };
	XMFLOAT3 ArmsRot = { 0.f,0.f,0.f };
	XMFLOAT3 ArmsScl = { 0.f,0.f,0.f };
	//影のステータス
	XMVECTOR ShadowPos = { 0,0,0 };
	XMFLOAT4 ShadowCol = { 0.0f,0.0f,0.0f,0.1f };

	int Hp = 160;
	int OldHp = 0;
};

