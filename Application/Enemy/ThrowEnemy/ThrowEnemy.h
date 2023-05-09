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
	enum State {
		APPEARANCE =0,
		WAIT,
		ATTACK,
		DEATH,
	};

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
	/// 登場処理
	/// </summary>
	void AppearanceProcess();

	void WaitProcess();
	/// <summary>
	/// 
	/// </summary>
	void AttackProcess(const XMFLOAT2& player2Dpos, bool& playerbulletshot);

	void DamageProcess(const XMFLOAT2& player2Dpos,bool& playerbulletshot);

	void DeathProcess();

	/// <summary>
	/// 2D→3D座標
	/// </summary>
	/// <param name="Set3dPosition">表示したい3D座標の場所</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& set3Dposition);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);

	void BulletCollision(const XMFLOAT2& player2Dpos, bool& playerbulletshot);

	void ThrowAttack();

	/// <summary>
	/// 倒されているか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return DeadFlag; }
public:
	//オブジェクト
	std::unique_ptr<Object3d>Center;
	std::unique_ptr<Object3d>bullet_;
	std::unique_ptr<Object3d>enemy_;
	std::unique_ptr<Object3d>propeller_;

	//スプライト
	std::unique_ptr<Sprite> RockOn;
	std::unique_ptr<Sprite> rockon_bullet_;
	//カメラ
	Camera* HaveCamera = nullptr;

	//パーツごとのステータス
	//敵の中心部分
	XMVECTOR CenterWorldPos = { 0.f,0.f,0.f };
	XMVECTOR center_pos_ = { 0.f,0.f,0.f };
	XMFLOAT3 CenterRot = { 0.f,0.f,0.f };
	XMMATRIX CenterMat;
	//
	XMVECTOR body_pos_ = {};
	XMFLOAT3 body_rot_ = {};
	XMFLOAT3 body_scl_ = { 1.f,1.f,1.f };
	//
	XMVECTOR propeller_pos_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_rot_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_scl_{ 1.f,1.f,1.f };
	//
	XMVECTOR old_pos_{};
	XMVECTOR bullet_pos_{};
	XMFLOAT3 bullet_rot_{};
	XMFLOAT3 bullet_scl_{};
	//着弾地点
	XMVECTOR LandingPoint = {};
	//2D座標を持たせる計算で使う変数
	XMVECTOR offset;
	XMMATRIX MatViewPort;
	//敵が持つ2D系のステータス
	XMFLOAT2 RockOnPos = { 0.0f,0.0f };
	XMFLOAT2 RockOnAnchorPoint = { 0.5f,0.5f };
	XMFLOAT4 RockOnCol = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockon_bulletpos_ = { 0.0f,0.0f };

	float bullet_speed_ = 0.05f;

	int Hp = 50;
	int OldHp = 0;
	float Length = 0.f;
	//敵とプレイヤーの距離
	float OriginDistance;
	float OriginHeadDistance;
	float OriginBoxDistance;
	float Distance = 60.0f;
	float bullet_distance_ = 0.f;
	bool bullet_active_ = true;

	//Hpが0以上か
	bool DeadFlag = false;

	float floating_pos_ = 0.f;

	int state_ = APPEARANCE;
};

