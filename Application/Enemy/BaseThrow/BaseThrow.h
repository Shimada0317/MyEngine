#pragma once
#include"Camera.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"ObjParticle.h"
#include"ParticleManager.h"
#include"Sprite.h"
#include"HelperMath.h"
#include"Action.h"

#include"ModelManager.h"
#include"SpriteManager.h"

#include<DirectXMath.h>
#include<list>

class Player;

class BaseThrow
{
protected:
	enum State {
		APPEARANCE = 0,
		WAIT,
		ATTACK,
		DEATH,
	}state_;

protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:

public:
	//仮想関数(共通の処理)

	virtual void Activity() = 0;
	

public:
	~BaseThrow();
	/// <summary>
	/// 初期か
	/// </summary>
	/// <param name="allrot">共通の角度</param>
	/// <param name="allpos">共通の座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="trackpos">追従先の座標</param>
	void Initialize(Camera* camera);
	/// <summary>
	/// ステータスのセット
	/// </summary>
	void StatusSet();
	/// <summary>
	/// 各更新処理をまとめる
	/// </summary>
	void AllUpdate();
	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="playerHp">プレイヤーのHP</param>
	void Update(Player* player);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);
	/// <summary>
	/// ダメージ処理
	/// </summary>
	void DamageProcess();
	/// <summary>
	/// 弾の当たり判定
	/// </summary>
	void BulletCollision();
	/// <summary>
	/// 投擲攻撃
	/// </summary>
	void ThrowAttack();
	/// <summary>
	/// パーティクル発生
	/// </summary>
	void ParticleEfect();

	/// <summary>
	/// 倒されているか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return dead_flag_; }

	void SetPlayer(Player* player) { player_ = player; }
protected:
	//オブジェクト
	unique_ptr<Object3d> center_;
	unique_ptr<Object3d> bullet_;
	unique_ptr<Object3d> enemy_;
	unique_ptr<Object3d> propeller_;
	Player* player_;
	list<unique_ptr<ObjParticle>> obj_particle_;
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
	//色
	XMFLOAT4 color_ = { 1.f,1.f,1.f,1.f };
	XMFLOAT4 bullet_color_ = { 1.f,0.f,0.f,1.f };
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
	float bullet_speed_ = 0.08f;
	int hp_ = 50;
	int oldhp_ = hp_;
	float length_ = 0.f;
	float sub_scl_ = -0.0009f;
	//敵とプレイヤーの距離
	float origin_distance_;
	float originhead_distance_;
	float originbox_distance_;
	float distance_ = 60.0f;
	float bullet_distance_ = 0.f;
	float bullet_magnification_ = 2.4f;
	bool bullet_active_ = true;
	//Hpが0以上か
	bool dead_flag_ = false;
	float floating_pos_ = 0.f;
	float fall_time_ = 0.f;
	//持ってきたプレイヤーの情報
	XMFLOAT2 player_pos_{};
	bool player_shot_ = false;
	int player_hp_ = 0;
	float add_value_ = 2.f;
	float timer_ = 0.f;
};

