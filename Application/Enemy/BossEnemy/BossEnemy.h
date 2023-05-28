#pragma once
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"

#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class Camera;

class BossEnemy
{
private: 
	enum State {
		kAppearance,
		kMove,
		kStun,
		kDead,
	}state_;
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//デストラクタ
	~BossEnemy();

	void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint);

	void StatusSet();

	void AllUpdate();

	void Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	void Draw(DirectXCommon* dxCommon);

	void Appearance();

	void Move(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	void Stun(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	/// <summary>
/// プレイヤー追尾モード
/// </summary>
	void TrackPlayerMode();

	void Damage();

	void Death();

	/// <summary>
	/// 2D→3D座標
	/// </summary>
	/// <param name="Set3dPosition">表示したい3D座標の場所</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& set3Dposition);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void ParticleEfect();

public:

	/// <summary>
/// 倒されているか
/// </summary>
/// <returns></returns>
	bool IsDead() const { return dead_flag_; }

private:
	//Obj
//頭
	std::unique_ptr<Object3d> headpart_;
	//体
	std::unique_ptr<Object3d> bodypart_;
	//足
	std::unique_ptr<Object3d> corepart_;
	//影
	std::unique_ptr<Object3d> shadow_;
	//中心
	std::unique_ptr<Object3d> center_;
	//Objパーティクル
	std::list<std::unique_ptr<ObjParticle>>obj_particle_;

	//スプライト
	//体
	std::unique_ptr<Sprite> rockon_;
	//頭
	std::unique_ptr<Sprite> rockonhead_;
	//コア
	std::unique_ptr<Sprite> rockoncore_;

	//ダメージを食らったときのエフェクト
	ParticleManager* partgreen_ = nullptr;
	ParticleManager* partred_ = nullptr;
	//カメラ
	Camera* bringupcamera_;
	//敵の中心部分のステータス
	XMVECTOR center_worldpos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 center_rot_ = { 0.0f,0.0f,0.0f };
	XMMATRIX center_mat_;
	//敵が持っているステータス
	int hp_ = 600;
	int oldhp_ = hp_;
	bool robotarive_flag_ = true;
	XMVECTOR track_point_ = { 0,0,0 };
	//パーツごとのスケール
	XMFLOAT3 headpart_scl_ = { 0.f,0.f,0.f };
	XMFLOAT3 bodypart_scl_ = { 0.f,0.f,0.f };
	XMFLOAT3 corepart_scl_ = { 0.f,0.f,0.f };
	//パーツごとのポジション
	XMVECTOR headpart_pos_ = { 0.0f,-1000.0f,0.0f };
	XMVECTOR bodypart_pos_ = { 0.0f,-1000.0f,0.0f };
	XMVECTOR corepart_pos_ = { 0.f,1000.f,0.f };
	//パーツごとの色
	XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 bodypart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 corepart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	//パーツごとの回転
	XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodypart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 corepart_rot_ = { 0.0f,0.0f,0.0f };
	//パーツごとに渡すステータス
	XMVECTOR all_pos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 all_rot_;

	//影のステータス
	XMVECTOR shadow_pos_ = {};
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };

	//敵が持つ2D系のステータス
	XMFLOAT2 rockon_pos_ = {};
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockonhead_pos_ = {};
	XMFLOAT2 rockoncore_pos_ = {};

	//2D座標を持たせる計算で使う変数
	XMVECTOR offset_;
	XMMATRIX matviewport_;

	float attack_charge_ = 0.0f;
	//移動速度
	float movespeed_ = 0.07f;
	//プレイヤーと敵の距離
	float length_ = 13.0f;
	float limit_length_ = 7.0f;
	//Hpが0以上か
	bool dead_flag_ = false;
	//敵とプレイヤーの距離
	float origin_distance_;
	float originhead_distance_;
	float origincore_distance_;
	float distance_ = 60.f;
	float head_distance_ = 30.f;
	float core_distance_=20.f;

	float limit_distance_ = 0.f;
	float atttack_timer_ = 0.f;
	//同じ追従先に別の敵がいるかいないか
	bool objparticle_flag_ = false;

	float addrot_ = 20.f;
	float time_ = 0.f;
	bool particleefect_flag_ = true;
};

