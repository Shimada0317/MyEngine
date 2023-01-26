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

class Robot
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	//デストラクタ
	~Robot();
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="allRot">回転</param>
	/// <param name="AllPos">座標</param>
	/// <param name="came">カメラ</param>
	/// <param name="Step">移動時に横移動するか</param>
	void Initialize(const XMFLOAT3& allRot, const XMVECTOR& AllPos, Camera* came, const bool& movement= false);

	void StatusSet();

	/// <summary>
	/// Upfdateはこの中で
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	void AllUpdata();
	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="playerHp">プレイヤーのHP</param>
	void Update(const XMFLOAT2& Player2DPos, int& PlayerHp,bool& PlayerBulletShot);
	//描画
	void Draw(DirectXCommon* dxCommon);
	//Img描画
	void ImgDraw();
	//パーティクル描画
	void ParticleDraw(DirectXCommon* dxCommon);
	//プレイヤー追尾モード
	void TrackPlayerMode();
	//首振り
	void Motion();

	void TrackRot(const XMFLOAT2& playerPos);
	//攻撃モード
	void AttackMode(int& playerHp);
	//攻撃
	void Attack(int& playerhp);
	/// <summary>
	/// 2D→3D座標
	/// </summary>
	/// <param name="Set3dPosition">表示したい3D座標の場所</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& Set3dPosition);
	//ビュー変換
	void ChangeViewPort(XMMATRIX& matViewPort);

public://Getter Setter
	void SetPosition(XMVECTOR allPos) { this->AllPos = allPos; }

	void SetRotation(XMFLOAT3 allRot) { this->AllRot = allRot; }

	void SetTrackPoint(XMVECTOR TrackPoint) { this->TrackPoint = TrackPoint; }

	const XMVECTOR& GetPosition() { return CenterWorldPos; }

	const XMFLOAT2& Get2DPosition() { return RockOnPos; }

	bool IsDead() const { return isDead_; }

private:
	//体ごとのパーツ


	//Obj
	std::unique_ptr<Object3d> HeadPart;
	std::unique_ptr<Object3d> ArmsPart;
	std::unique_ptr<Object3d> BodyPart;
	std::unique_ptr<Object3d> Shadow;
	std::unique_ptr<Object3d> Center;
	std::unique_ptr<Object3d> HeadCenter;
	//Objモデル
	ObjModel* ShadowModel = nullptr;
	ObjModel* HeadPartModel;
	ObjModel* ArmsPartModel;
	ObjModel* BodyPartModel;
	//Objパーティクル
	std::list<std::unique_ptr<ObjParticle>>Obj_Particle;
	//スプライト
	std::unique_ptr<Sprite> RockOn;
	std::unique_ptr<Sprite> RockOnHead;
	//その他
	Audio* ClushSe;
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

	float Distance = 60.0f;
};

