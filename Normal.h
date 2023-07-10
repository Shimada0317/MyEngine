#pragma once
#include "Base.h"
class Normal :
    public Base
{
private:
	enum State {
		kDefomation,
		kMove,
		kWait,
		kAttack,
		kDeath,
	}state_;

	enum Type {
		kNormal,
		kRapid,
	};
private:
	//状態遷移の関数ポインタ
	static void (Normal::* StateFuncTable[])();
public:
    Normal(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint);

    void CreateRobot(Camera* camera)override;

    void StatusSet();

    void AllUpdate();

    void Activity(Player* player)override;

    void Draw(DirectXCommon* dxCommon)override;
private:
	/// <summary>
	/// 変形
	/// </summary>
	void Defomation();
	/// <summary>
	/// プレイヤー追尾モード
	/// </summary>
	void TrackPlayerMode();
	/// <summary>
	/// 到着後の待機
	/// </summary>
	void WaitMode();
	/// <summary>
	/// 待機後の攻撃
	/// </summary>
	void AttackMode();

	void Death();
private:
	/// <summary>
	/// 展開時の大きさ
	/// </summary>
	void DeploymentScale();
	/// <summary>
	/// 追従する時、先に他の敵がいるか
	/// </summary>
	/// <param name="otherenemyarive">自信以外が先にいるか</param>
	void WaitTrack(bool otherenemyarive);
	/// <summary>
	/// パーツを大きくする
	/// </summary>
	void Enlargement();
	/// <summary>
	/// 揺らす処理
	/// </summary>
	void ShakeBody();
	/// <summary>
	/// 攻撃チャージ
	/// </summary>
	void AttackCharge();
	/// <summary>
	/// 
	/// </summary>
	void AttackBefore();
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	/// <summary>
	/// 攻撃を受けたときに色を変える
	/// </summary>
	void HitColor();
	/// <summary>
	/// 透明にする
	/// </summary>
	void Transparentize();

	void EnemyCollision();
private:
    //Obj
    //頭
    std::unique_ptr<Object3d> head_part_;
    //両腕
    std::unique_ptr<Object3d> arms_part_;
    //体
    std::unique_ptr<Object3d> body_part_;
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
	//振動
	float vibration_ = 0.0f;
	bool vibrationchange_flag_ = false;
	float atttack_timer_ = 0.f;
	float timer_limit_ = 0.f;
	//敵のモーション用
	float purse_positiverot_ = 180;
	//敵とプレイヤーの距離
	float originhead_distance_;
	float head_distance_ = 30.0f;
	//変形
	float defomation_count_ = 0.0f;

	float limit_length_ = 1.2f;
	//同じ追従先に別の敵がいるかいないか
	bool wait_flag_ = false;
	float attack_charge_ = 0.0f;
};

