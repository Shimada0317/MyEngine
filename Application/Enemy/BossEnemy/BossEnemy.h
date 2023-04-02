#pragma once
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include"Camera.h"
#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class BossEnemy
{
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

	/// <summary>
/// プレイヤー追尾モード
/// </summary>
	void TrackPlayerMode();

	/// <summary>
	/// 攻撃モード
	/// </summary>
	/// <param name="playerHp">プレイヤーのHp</param>
	void AttackMode(int& playerhp);

	/// <summary>
	/// 攻撃
	/// </summary>
	/// <param name="playerhp">プレイヤーのHp</param>
	void Attack(int& playerhp, float& attacktimer);

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

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void SelfDestructingEfect();

	void WaitTrack(bool otherenemyarive);
public:

	/// <summary>
/// 倒されているか
/// </summary>
/// <returns></returns>
	bool IsDead() const { return DeadFlag; }

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
	Camera* GetCamera;
	//敵の中心部分のステータス
	XMVECTOR CenterWorldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 CenterRot = { 0.0f,0.0f,0.0f };
	XMMATRIX CenterMat;

	//敵が持っているステータス
	int Hp = 50;
	int OldHp = 0;
	bool RobotAriveFlag = false;
	XMVECTOR TrackPoint = { 0,0,0 };
	XMVECTOR OldTrackPoint = { 0,0,0 };
	XMVECTOR FakeTrackPoint = { 0,0,0 };

	//パーツごとのスケール
	XMFLOAT3 HeadPartScl = { 1.2f,1.2f,1.2f };
	XMFLOAT3 BodyPartScl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 ArmsPartScl = { 0.8f,0.8f,0.8f };

	//パーツごとのポジション
	XMVECTOR HeadPartPos = { 0.0f,-1000.0f,0.0f };
	XMVECTOR BodyPartPos = { 0.0f,-1000.0f,0.0f };
	XMVECTOR ArmsPartPos = { 0.0f,-1000.0f,0.0f };

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

	//攻撃モードで使用される変数
	bool AttackFaseFlag = false;

	//攻撃の準備時間
	bool AttackShakeDownFlag = false;
	float AttackCharge = 0.0f;

	//移動速度
	float MoveSpeed = 0.03f;

	//プレイヤーと敵の距離
	float Length = 13.0f;
	float LengthLimit = 7.0f;

	//待機時のモーション変数
	float MotionTime = 0.0f;
	bool MotionChangeFlag = true;

	//Hpが0以上か
	bool DeadFlag = false;

	//敵とプレイヤーの距離
	float OriginDistance;
	float OriginHeadDistance;

	float Distance = 60.0f;
	float HeadDistance = 30.0f;

	//敵のモーション用
	float PursePositiveRot = 180;
	float PurseNegativeeRot = 0;

	//変形用のフラグ
	bool DefomationFlag = false;
	float DefomationCount = 0.0f;

	//振動
	float Vibration = 0.0f;
	bool VibrationChangeFlag = false;

	XMVECTOR AttackBeforePos = { 0.0f,0.0f,0.0f };
	float LimitDistance = 0.0f;

	float AtttackTimer = 0.f;
	float TimerLimit = 0.f;
	bool RandomFlag = false;
	float AttackTimeMin = 20;
	float AttackTimeMax = 40;

	bool ParticleEfectFlag = true;
	//同じ追従先に別の敵がいるかいないか
	bool WaitFlag = false;

	bool NotLifeFlag = false;

	bool ObjParticleFlag = false;

};

