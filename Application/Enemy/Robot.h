#pragma once
#include"Audio.h"
#include"Body.h"
#include"BothArms.h"
#include"Head.h"
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
	/// <summary>
	/// Upfdateはこの中で
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	void AllUpdata(Bullet* bull);
	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="playerHp">プレイヤーのHP</param>
	void Update(Bullet* bull,int& playerHp);
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
	//攻撃モード
	void AttackMode(int& playerHp);
	//2D→3D座標
	void WorldtoScreen();
	//ビュー変換
	void ChangeViewPort(XMMATRIX& matViewPort);

public://Getter Setter
	void SetPosition(XMVECTOR allPos) { this->All_Pos = allPos; }

	void SetRotation(XMFLOAT3 allRot) { this->All_Rot = allRot; }

	void SetTrackPoint(XMVECTOR TrackPoint) { this->TrackPoint = TrackPoint; }

	const XMVECTOR& GetPosition() { return Center_WorldPos; }

	const XMFLOAT2& Get2DPosition() { return RockOn_Pos; }

	bool IsDead() const { return isDead_; }

private:
	//体ごとのパーツ
	std::unique_ptr<Head> head;
	std::unique_ptr<BothArms>arms;
	std::unique_ptr<Body>body;

	std::unique_ptr<Object3d> HeadPart;
	std::unique_ptr<Object3d> ArmsPart;
	std::unique_ptr<Object3d> BodyPart;

	//Obj
	std::unique_ptr<Object3d> Shadow;
	std::unique_ptr<Object3d> Center;
	//Objモデル
	ObjModel* ShadowModel = nullptr;
	ObjModel* HeadPartModel;
	ObjModel* ArmsPartModel;
	ObjModel* BodyPartModel;
	//Objパーティクル
	std::list<std::unique_ptr<ObjParticle>>Obj_Particle;
	//スプライト
	std::unique_ptr<Sprite> RockOn;
	//その他
	Audio* Clush_SE;
	Camera* camera;
	//敵の中心部分のステータス
	XMVECTOR Center_WorldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Center_Rot = { 0.0f,0.0f,0.0f };
	//敵が持っているステータス
	int Hp = 50;
	int OldHp = 0;
	bool Robotarive = false;
	XMVECTOR TrackPoint = { 0,0,0 };
	//パーツごとのスケール
	XMFLOAT3 HeadPartScl = { 0.3f,0.3f,0.3f };
	XMFLOAT3 BodyPartScl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 ArmsPartScl = { 0.2f,0.2f,0.2f };
	XMVECTOR HeadPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR BodyPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR ArmsPartPos = { 0.0f,0.0f,0.0f };
		//パーツごとに渡すステータス
		XMVECTOR All_Pos = { 0.0f,0.0f,-10.0f };
		XMFLOAT3 All_Rot;
		//部位破壊用
		bool RemainPart[3];
	//影のステータス
	XMVECTOR Shadow_Pos = { 0,0,0 };
	XMFLOAT4 Shadow_Col = { 0.0f,0.0f,0.0f,0.1f };
	//敵が持つ2D系のステータス
	XMFLOAT2 RockOn_Pos = { 0.0f,0.0f };
	XMFLOAT2 RockOn_Anc = { 0.5f,0.5f };
	XMFLOAT4 RockOn_Col = { 1.0f,1.0f,1.0f,1.0f };
	//2D座標を持たせる計算で使う変数
	XMVECTOR offset;
	XMMATRIX Center_Mat;
	XMMATRIX MatViewPort;
	//攻撃モードで使用される変数
	float AttackTime = 0.0f;
	float AttackChanse = 0;
	float Rand = 0;
	bool AttackFase = false;
	float AttackPreparationTime = 0;
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
};

