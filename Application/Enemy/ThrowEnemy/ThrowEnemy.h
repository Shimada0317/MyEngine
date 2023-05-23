#pragma once
#include"Camera.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"ObjParticle.h"
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
	void AttackProcess(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

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

	void ThrowAttack(int& playerhp);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void ParticleEfect();

	/// <summary>
	/// 倒されているか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return dead_flag_; }
public:
	//オブジェクト
	unique_ptr<Object3d>center_;
	unique_ptr<Object3d>bullet_;
	unique_ptr<Object3d>enemy_;
	unique_ptr<Object3d>propeller_;
	list<unique_ptr<ObjParticle>>obj_particle_;
	//スプライト
	unique_ptr<Sprite> rockon_;
	unique_ptr<Sprite> rockon_bullet_;
	//カメラ
	Camera* bringupcamera_;
	//ダメージを食らったときのエフェクト
	ParticleManager* partgreen_ = nullptr;
	ParticleManager* partred_ = nullptr;
	//パーツごとのステータス
	//敵の中心部分
	XMVECTOR center_worldpos_ = { 0.f,0.f,0.f };
	XMVECTOR center_pos_ = { 0.f,0.f,0.f };
	XMFLOAT3 center_rot_ = { 0.f,0.f,0.f };
	XMMATRIX center_mat_;
	//本体のステータス
	XMVECTOR body_pos_ = {};
	XMFLOAT3 body_rot_ = {};
	XMFLOAT3 body_scl_ = { 1.f,1.f,1.f };
	//羽のステータス
	XMVECTOR propeller_pos_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_rot_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_scl_{ 1.f,1.f,1.f };
	//弾のステータス
	XMVECTOR old_pos_{};
	XMVECTOR bullet_pos_{};
	XMFLOAT3 bullet_rot_{};
	XMFLOAT3 bullet_scl_{};
	//着弾地点
	XMVECTOR landing_point_ = {};
	//2D座標を持たせる計算で使う変数
	XMVECTOR offset_;
	XMMATRIX matviewport_;
	//敵が持つ2D系のステータス
	XMFLOAT2 rockon_pos_ = { 0.0f,0.0f };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockon_bulletpos_ = { 0.0f,0.0f };
	//弾速
	float bullet_speed_ = 0.05f;
	int hp_ = 50;
	int oldhp_ = hp_;
	float length_ = 0.f;
	//敵とプレイヤーの距離
	float origin_distance_;
	float originhead_distance_;
	float originbox_distance_;
	float distance_ = 60.0f;
	float bullet_distance_ = 0.f;
	bool bullet_active_ = true;
	//Hpが0以上か
	bool dead_flag_ = false;
	bool particle_flag_ = true;
	float floating_pos_ = 0.f;
	float fall_time_ = 0.f;
	int state_ = APPEARANCE;
};

