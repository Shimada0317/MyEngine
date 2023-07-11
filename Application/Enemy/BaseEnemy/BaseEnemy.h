#pragma once
#include"Action.h"
#include"Audio.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include"ParticleManager.h"
#include"Collision.h"

using namespace DirectX;

class Player;

class BaseEnemy
{

protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://共通の仮想関数
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="came">カメラ</param>
	virtual void CreateRobot(Camera* camera) = 0;
	
	
	virtual void Activity(Player* player) = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	virtual void Draw(DirectXCommon* dxCommon) = 0;

	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Update(Player* player);

	bool IsDead()const { return dead_flag_; }
protected://各クラス内で使われる処理
	

	void CommonLoad();

	void CommonStatusSet();

	void CommonUpdate();

	void GetPlayerState(Player* player);

	void ObjParticleDelete();

	void CommonCollision();

	void TrackCalculation();

	void CommonParticleDraw(DirectXCommon* dxCommon);

	void CommonObjDraw();

	void ParticleEffect();

	void Damage();
protected://共通の変数
	//影
	std::unique_ptr<Object3d> shadow_;
	//中心
	std::unique_ptr<Object3d> center_;
		//スプライト
	unique_ptr<Sprite> rockon_body_;
	unique_ptr<Sprite> rockon_other_;
	//Objパーティクル
	std::list<std::unique_ptr<ObjParticle>>obj_particle_;
	//ダメージを食らったときのエフェクト
	ParticleManager* partgreen_ = nullptr;
	ParticleManager* partred_ = nullptr;
	//カメラ
	Camera* bringupcamera_;

	//追尾の計算
	XMFLOAT3 value_;
	//体の中心の座標
	XMVECTOR base_pos_{};
	XMFLOAT3 all_rot_;
	XMFLOAT3 base_rot_{};
	//影のステータス
	XMVECTOR shadow_pos_ {};
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };
	//敵の中心部分
	XMVECTOR center_worldpos_ {};
	XMFLOAT3 center_rot_ {};
	XMMATRIX center_mat_;
	//敵が持つ2D系のステータス
	XMFLOAT2 rockon_pos_ {};
	XMFLOAT2 rockonother_pos_ {};
	XMFLOAT2 anchorpoint_ {0.5f,0.5f };
	XMFLOAT4 rockon_color_ {1.f,1.f,1.f,1.f};
	//敵が持っているステータス
	int hp_ = 50;
	int old_hp_ = 0;
	float length_ = 0.f;
	float mul_value_ = 0.f;
	int damage_value_ = 0;
	//移動速度
	float speed_ = 0.f;
	XMVECTOR TrackSpeed{};
	XMVECTOR track_point_ {};
	XMVECTOR oldtrack_point_ {};
	//持ってきたプレイヤーの情報
	Player* player_;
	XMFLOAT2 player_pos_{};
	bool player_shot_ = false;
	int player_hp_ = 5;
	//敵とプレイヤーの距離
	float origin_distance_;
	float originother_distance_;
	float originbox_distance_;
	float distance_ = 60.0f;
	//Hpが0以上か
	bool dead_flag_ = false;

	float start_scl_ = 4.f;
	float end_scl_ = 0.f;
	float time_ = 150.f;
	int roop_count_ = 50;
	int file = 200;
};

