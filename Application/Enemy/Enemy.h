#pragma once
#include"Audio.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Texture.h"
#include"TextureModel.h"
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
public:
	//デストラクタ
	~Enemy();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="allRot">回転</param>
	/// <param name="AllPos">座標</param>
	/// <param name="came">カメラ</param>
	/// <param name="Step">移動時に横移動するか</param>
	void Initialize(const XMFLOAT3& allRot, const XMVECTOR& AllPos, Camera* came, const bool& movement= false);

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
	void Update(const XMFLOAT2& Player2DPos, int& PlayerHp,bool& PlayerBulletShot);

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
	void Attack(int& playerhp);

	/// <summary>
	/// 2D→3D座標
	/// </summary>
	/// <param name="Set3dPosition">表示したい3D座標の場所</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& Set3dPosition);

	/// <summary>
	/// ビュー変換
	/// </summary>
	void ChangeViewPort(XMMATRIX& matViewPort);

public://Getter Setter
	/// <summary>
	/// 追尾先のセッター
	/// </summary>
	/// <param name="TrackPoint">追尾先</param>
	void SetTrackPoint(const XMVECTOR& TrackPoint) { this->TrackPoint = TrackPoint; }

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
	bool IsDead() const { return isDead_; }

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

	//Objモデル
	//影モデル
	ObjModel* ShadowModel = nullptr;
	//頭モデル
	ObjModel* HeadPartModel;
	//両腕モデル
	ObjModel* ArmsPartModel;
	//体モデル
	ObjModel* BodyPartModel;

	//Objパーティクル
	std::list<std::unique_ptr<ObjParticle>>Obj_Particle;

	//スプライト
	//体
	std::unique_ptr<Sprite> RockOn;
	//頭
	std::unique_ptr<Sprite> RockOnHead;

	//その他
	//サウンドエフェクト
	Audio* ClushSe;
	//カメラ
	Camera* camera;

	//敵の中心部分のステータス
	XMVECTOR CenterWorldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 CenterRot = { 0.0f,0.0f,0.0f };
	XMMATRIX CenterMat;

	//敵が持っているステータス
	int Hp = 50;
	int OldHp = 0;
	bool RobotArive = false;
	XMVECTOR TrackPoint = { 0,0,0 };

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
	bool AttackFase = false;

	//攻撃の準備時間
	float AttackPreparationTime = 0;
	bool AttackShakeDown = false;

	//移動速度
	float MoveSpeed = 0.005f;

	//プレイヤーと敵の距離
	float Length = 0;

	//待機時のモーション変数
	float MotionTime = 0.0f;
	bool MotionChange = true;

	//サイドステップ
	float MovementChangeTime = 0.0f;
	bool Reversal_F = false;
	float SideStepSpeed = 0.001f;
	bool Movement_F = false;

	//Hpが0以上か
	bool isDead_ = false;

	//敵とプレイヤーの距離
	float OriginDistance;
	float OriginHeadDistance;

	float Distance = 60.0f;
	float HeadDistance = 30.0f;

	//敵のモーション用
	float PursePositiveRot = 180;
	float PurseNegativeeRot = 0;
};

