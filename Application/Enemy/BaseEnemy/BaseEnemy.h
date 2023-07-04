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
	enum State {
		kDefomation,
		kMove,
		kWait,
		kAttack,
		kDeath,
	}state_;
protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//仮想関数

	virtual void Activity() = 0;
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="came">カメラ</param>
	void Initialize(Camera* camera);
	/// <summary>
	/// ステータスをセット
	/// </summary>
	void StatusSet();
	/// <summary>
	/// Obj等のUpdateはこの中で
	/// </summary>
	void AllUpdate();
	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Update(Player* player);
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);
protected:
	/// <summary>
	/// プレイヤー追尾モード
	/// </summary>
	void TrackPlayerMode();
	/// <summary>
/// ダメージを食らった
/// </summary>
	void Damage();
	/// <summary>
	/// 死んだ
	/// </summary>
	void Death();

	bool CheckHit(bool& playershot, XMFLOAT2 playerppos, XMFLOAT2 rockonpos, float distance, float mulvalue);

	void ParticleEfect();
	/// <summary>
	/// 透明にする
	/// </summary>
	void Tracsparentsize();
	/// <summary>
	/// 攻撃を受けたときに色を変える
	/// </summary>
	void HitColor();
	/// <summary>
	/// 攻撃チャージ
	/// </summary>
	void AttackCharge();
	/// <summary>
	/// 揺らす処理
	/// </summary>
	void ShakeBody();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	/// <summary>
	/// 透明にする
	/// </summary>
	void Transparentize();
	/// <summary>
	/// パーツを大きくする
	/// </summary>
	void Enlargement();

	void AttackBefore();
	/// <summary>
	/// 展開時の大きさ
	/// </summary>
	void DeploymentScale();
private:

	bool CheckShot(bool playershot);
public://Getter Setter
/// <summary>
/// 追尾先のセッター
/// </summary>
/// <param name="TrackPoint">追尾先</param>
	void SetTrackPoint(const XMVECTOR& trackpoint) { this->oldtrack_point_ = trackpoint; }
	/// <summary>
	/// 追尾先のゲッター
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetTrackPos() { return oldtrack_point_; }

	const XMVECTOR& CheckTrackPoint() { return track_point_; }
	/// <summary>
	/// 中心のワールド座標のゲッター
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetPosition() { return center_worldpos_; }
	/// <summary>
	/// 倒されているか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return dead_flag_; }

protected:
	//Obj
	//頭
	std::unique_ptr<Object3d> headpart_;
	//両腕
	std::unique_ptr<Object3d> armspart_;
	//体
	std::unique_ptr<Object3d> bodypart_;
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
	//ダメージを食らったときのエフェクト
	ParticleManager* partgreen_ = nullptr;
	ParticleManager* partred_ = nullptr;
	//その他
	//カメラ
	Camera* bringupcamera_;
	//敵の中心部分のステータス
	XMVECTOR center_worldpos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 center_rot_ = { 0.0f,0.0f,0.0f };
	XMMATRIX center_mat_;
	//敵が持っているステータス
	int base_hp_ = 50;
	int old_hp_ = 0;
	XMVECTOR track_point_ = { 0,0,0 };
	XMVECTOR oldtrack_point_ = { 0,0,0 };
	//パーツごとのスケール
	XMFLOAT3 headpart_scl_ = { 0.3f,0.3f,0.3f };
	XMFLOAT3 bodypart_scl_ = { 1.0f,1.0f,1.0f };
	XMFLOAT3 armspart_scl_ = { 0.2f,0.2f,0.2f };
	//パーツごとのポジション
	XMVECTOR headpart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR bodypart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR armspart_pos_ = { 0.0f,0.0f,0.0f };
	//パーツごとの色
	XMFLOAT4 armspart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 bodypart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	//パーツごとの回転
	XMFLOAT3 armspart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodypart_rot_ = { 0.0f,0.0f,0.0f };
	//パーツごとに渡すステータス
	XMVECTOR base_pos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 all_rot_;
	//影のステータス
	XMVECTOR shadow_pos_ = { 0,0,0 };
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };
	//敵が持つ2D系のステータス
	XMFLOAT2 rockon_pos_ = { 0.0f,0.0f };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockonhead_pos_ = { 0.0f,0.0f };
	float attack_charge_ = 0.0f;
	//移動速度
	float movespeed_ = 0.09f;
	//プレイヤーと敵の距離
	float length_ = 3.0f;
	float limit_length_ = 1.2f;
	//死んでいるか
	bool dead_flag_ = false;
	//敵とプレイヤーの距離
	float origin_distance_;
	float originhead_distance_;
	float distance_ = 60.0f;
	float head_distance_ = 30.0f;
	//敵のモーション用
	float purse_positiverot_ = 180;
	//変形用のフラグ
	float defomation_count_ = 0.0f;
	//振動
	float vibration_ = 0.0f;
	bool vibrationchange_flag_ = false;
	float atttack_timer_ = 0.f;
	float timer_limit_ = 0.f;
	//同じ追従先に別の敵がいるかいないか
	bool wait_flag_ = false;
	//持ってきたプレイヤーの情報
	Player* player_;
	XMFLOAT2 player_pos_{};
	int player_hp_ = 0;
	bool particle_flag_ = true;
};

