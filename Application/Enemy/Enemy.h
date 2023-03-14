#pragma once
#include"Audio.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include"Camera.h"
#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class Enemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//デストラクタ
	~Enemy();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="allRot">回転</param>
	/// <param name="allPos">座標</param>
	/// <param name="came">カメラ</param>
	/// <param name="Step">移動時に横移動するか</param>
	void Initialize(const XMFLOAT3& allRot, const XMVECTOR& allPos, Camera* camera, const XMVECTOR& trackpoint, const bool& movement= false);

	/// <summary>
	/// ステータスをセット
	/// </summary>
	void StatusSet();

	/// <summary>
	/// Obj等のUpdateはこの中で
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	void AllUpdate();

	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="playerHp">プレイヤーのHP</param>
	void Update(const XMFLOAT2& player2Dpos, int& playerhp,bool& playerbulletshot);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
	/// プレイヤー追尾モード
	/// </summary>
	void TrackPlayerMode();

	/// <summary>
	/// 待機モーション
	/// </summary>
	void Motion();

	/// <summary>
	/// 攻撃モード
	/// </summary>
	/// <param name="playerHp">プレイヤーのHp</param>
	void AttackMode(int& playerHp);

	/// <summary>
	/// 攻撃
	/// </summary>
	/// <param name="playerhp">プレイヤーのHp</param>
	void Attack(int& playerhp,float& attacktimer);

	/// <summary>
	/// 2D→3D座標
	/// </summary>
	/// <param name="Set3dPosition">表示したい3D座標の場所</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& set3Dposition);

	/// <summary>
	/// ビュー変換
	/// </summary>
	void ChangeViewPort(XMMATRIX& matviewport);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void ParticleEfect();

	void WaitTrack(bool otherenemyarive);

public://Getter Setter
	/// <summary>
	/// 追尾先のセッター
	/// </summary>
	/// <param name="TrackPoint">追尾先</param>
	void SetTrackPoint(const XMVECTOR& trackpoint) { this->OldTrackPoint = trackpoint; }

	/// <summary>
	/// 追尾先のゲッター
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetTrackPos() { return OldTrackPoint; }

	/// <summary>
	/// 中心のワールド座標のゲッター
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetPosition() { return CenterWorldPos; }

	/// <summary>
	/// 2D座標のゲッター
	/// </summary>
	/// <returns></returns>
	const XMFLOAT2& Get2DPosition() { return RockOnPos; }

	/// <summary>
	/// 倒されているか
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return DeadFlag; }

	bool GetArive() { return RobotAriveFlag; }

	bool GetCountZero() { return CountZeroFlag; }

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

	//その他
	//サウンドエフェクト
	Audio* ClushSe;
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
	XMVECTOR OldTrackPoint = { 0,0,0 };
	XMVECTOR FakeTrackPoint = { 0,0,0 };

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

	//攻撃モードで使用される変数
	float AttackTime = 0.0f;
	int AttackChanse = 0;
	float Rand = 0;
	bool AttackFaseFlag = false;

	//攻撃の準備時間
	float AttackPreparationTime = 0;
	bool AttackShakeDownFlag = false;
	float AttackCharge = 0.0f;

	//移動速度
	float MoveSpeed = 0.09f;

	//プレイヤーと敵の距離
	float Length = 3.0f;
	float LengthLimit = 1.5f;

	//待機時のモーション変数
	float MotionTime = 0.0f;
	bool MotionChangeFlag = true;

	//サイドステップ
	float MovementChangeTime = 0.0f;
	bool ReversalFlag = false;
	float SideStepSpeed = 0.001f;
	bool MovementFlag = false;

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
	float Vibration=0.0f;
	bool VibrationChangeFlag = false;

	XMVECTOR AttackBeforePos = { 0.0f,0.0f,0.0f };
	float LimitDistance = 0.0f;

	float AtttackTimer = 0.f;
	float TimerLimit = 0.f;
	bool RandomFlag = false;
	float AttackTimeMin = 20;
	float AttackTimeMax = 40;

	bool ParticleEfectFlag = false;
	//同じ追従先に別の敵がいるかいないか
	bool WaitFlag = false;

	bool CountZeroFlag = false;
};

