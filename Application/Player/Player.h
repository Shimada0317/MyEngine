#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Sprite.h"
#include<vector>
#include"Audio.h"
#include"Bullet.h"
#include"ParticleManager.h"
#include"RailCamera.h"

#include<memory>
#include<list>

class Player
{
public:
	~Player();
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="came">カメラ</param>
	void Initalize(Camera* camera);
	/// <summary>
	/// ステータスのSetは全部ここ
	/// </summary>
	/// <param name="came"></param>
	void StatusSet(Camera* camera);
	/// <summary>
	///　オブジェクト等の更新処理
	/// </summary>
	void AllUpdate(Camera* camera);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="Remaining">残弾数</param>
	/// <param name="enePos">敵の座標</param>
	/// <param name="came">カメラ</param>
	/// <param name="Ene2dPos">敵の2D座標</param>
	/// <param name="pat">現在のフェイズ</param>
	void Update(Camera* camera,int patern);
	/// <summary>
	/// パーティクルの描画
	/// </summary>
	/// <param name="cmdeList">コマンドリスト</param>
	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);
	/// <summary>
	/// スプライトの描画
	/// </summary>
	void SpriteDraw();
	/// <summary>
	/// スタート時のカメラワーク
	/// </summary>
	void CameraWork();
	/// <summary>
	/// 敵を倒した時の移動
	/// </summary>
	/// <param name="move"></param>
	/// <param name="patern">フェイズ番号</param>
	void PlayerMove(bool& move,int patern);
	/// <summary>
	/// Objの描画
	/// </summary>
	void ObjDraw();
	/// <summary>
	/// Imguiの描画
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// ビューポート行列の計算
	/// </summary>
	/// <param name="Track_Mat">追尾用の行列</param>
	void ChangeViewPort(XMMATRIX& Track_Mat);
	/// <summary>
	/// SE呼び出し
	/// </summary>
	void SoundEffect();
	/// <summary>
	/// マウスの取得
	/// </summary>
	void MouthContoroll();
	/// <summary>
	/// 画面揺れ
	/// </summary>
	/// <param name="limitshakevalue">シェイクする値</param>
	void ScreenShake(float shakevalue,float shakingtime);
	
	void DamageProcess();

	void GunShotProcess();

	void ReloadProcess();

	/// <summary>
	/// パーティクル発生
	/// </summary>
	void ParticleEfect();

#pragma region Get
	const bool& GetFinish() { return FinishFlag; }
	//三次元座標
	const XMVECTOR& GetPosition() { return ReticlePos; }
	//角度
	const XMFLOAT3& GetRotation() { return ReticleRot; }
	//スケール
	const XMFLOAT3& GetScl() { return ReticleScl; }
	//二次元座標
	const XMFLOAT2& GetRetPosition() { return ReticlePos2D; }
	//二次元スケール
	const XMFLOAT2& GetRetSiz() { return ReticleSize; }
	//カメラワーク
	const bool& GetCamWork() { return CameraWorkFlag; }

	const bool& GetFring() { return FringFlag; }

	//Hp
	int GetHp() { return Hp; }

	const bool& GetBulletShot() { return BulletShotFlag; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->ReticlePos = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->ReticleRot = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->ReticleScl = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->ReticlePos2D = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->ReticleSize = scale; }
	void SetHp(int HP) { this->Hp = HP; }
	void SetFinish(const bool& finish) { this->FinishFlag = finish; }
	void SetBulletShot(const bool& BulletShot_F) { this->BulletShotFlag = BulletShot_F; }
#pragma endregion
private:
	//Obj
	std::unique_ptr<Object3d> Track;
	std::unique_ptr<Object3d> Gun;
	std::unique_ptr<Object3d> Body;
	std::unique_ptr<Object3d> Cartridge;
	//スプライト
	std::unique_ptr<Sprite> SpriteReticle;
	std::unique_ptr<Sprite> CurtainUp;
	std::unique_ptr<Sprite> CurtainDown;
	std::unique_ptr<Sprite> Window;
	std::unique_ptr<Sprite> Skip;
	std::unique_ptr<Sprite> bulletHUD[9];
	std::unique_ptr<Sprite> Smoke[9];
	std::unique_ptr<Sprite> Reload;
	//発砲時のエフェクト
	ParticleManager* PartGreen = nullptr;
	ParticleManager* PartRed = nullptr;
	ParticleManager* PartSmoke = nullptr;
	//その他の機能
	Audio* ShotSe;
	Audio* ReloadSe;
	RailCamera* RailCam;
	//ローカル
		//レティクルObjのステータス
		XMVECTOR ReticlePos = { 0.0f,55.0f,0.0f };
		XMFLOAT3 ReticleRot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 ReticleScl = { 0.3f,0.3f,0.3f };
		//発射台Objのステータス
		XMVECTOR GunPos = { 0.0f,0.0f,-3.0f };
		XMVECTOR GunNotParentPos = { 0.0f,0.0f,-10.0f };
		XMFLOAT3 GunRot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 GunScl={0.3f,0.3f,0.3f};
		//薬莢Objステータス
		XMVECTOR CartridgePos = { 0.0f,0.0f,0.0f };
		XMFLOAT3 CartridgeRot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 CartridgeScl = { 0.1f,0.1f,0.1f };
		//プレイヤーObjのステータス
		XMVECTOR BodyPos = { 0.0f,0.0f,0.0f };
		XMFLOAT3 BodyRot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 BodyScl = { 0.3f,0.3f,0.3f };
	//ワールド
		//レティクルのステータス
		XMVECTOR TrackWorldPos = { 0.0f,0.0f,0.0f };
		XMMATRIX TrackMat;
		XMVECTOR WorldFarPos={0.0f,0.0f,0.0f};
		//発射台のステータス
		XMVECTOR GunWorldPos = { 0.0f,0.0f,-5.0f };
		XMMATRIX GunMat;
		//プレイヤーのステータス
		XMVECTOR BodyWorldPos = { 0.0f,0.0f,-0.1f };
		XMMATRIX BodyMat;
		XMVECTOR ReticleWorldPos;
		//パーティクルのステータス
		XMVECTOR ParticlePos = { 0.0f,0.0f,2.0f };
	//ビューポート
	XMMATRIX MatViewPort;
	//カメラ
	XMFLOAT3 EyeRot = {0.0f,0.0f,0.0f};
	XMFLOAT3 TargetPos = {0.0f,0.0f,0.0f};
	XMFLOAT3 Up = { 0.0f,1.0f,0.0f };
	//スプライト
		//2Dレティクルのステータス
		XMFLOAT4 ReticleColor = { 1,1,1,1 };
		XMFLOAT2 ReticleAncorPoint = { 0.5f,0.5f };
		XMFLOAT2 ReticlePos2D = { 640.0f,360.0f };
		XMFLOAT2 ReticleSize = { 64.0f,64.0f };
		//始まりと終わりの演出使う黒いカーテン
		XMFLOAT2 CurtainUpPos = { 0.0f,0.0f };
		XMFLOAT2 CurtainSize = { 1280.0f,100.0f };
		XMFLOAT2 CurtainDownPos = { 0.0f,620.0f };
		//Skip文字の表示
		XMFLOAT2 SkipPos = { 1100,720.0f };
	//2D座標入の取得用変数
	XMVECTOR offset = { 0,0,1.0f };

	bool ParticleFlag = false;
	int Hp = 5;
	int OldHp = 0;
	//Reload
	bool ReloadFlag = false;
	int ReloadTime = 0;
	int Anser = 0;
	//移動
	XMVECTOR Velocity;
	bool MoveFlag = false;
	//弾の速度
	float MoveSpeed=0;
	//動いたときのtimer
	float MoveTimer = 0.0f;
	int WaveCount = 0;

	//cam
	int shake = 0;
	float ShakingValue = 0.0f;

	bool FinishFlag = false;

	bool CameraWorkFlag = false;

	bool MovieFlag = false;
	bool StanbyFlag = false;
	int ActionCount = 0;
	float ActionTimer = 0.0f;

	bool StartFlag = false;

	bool FringFlag = false;

	bool ShakeHeadFlag = true;

	bool ReloadSoundFlag = true;

	float ChangeRot = 0;

	int EnemyCount = 0;

	int PaternCount = 0;

	const float  M_PI = 3.14;

	bool BulletShotFlag = false;
	float ShotCoolTime = 0.0f;
	bool RecoilFlag = false;
	float RecoilTime = 0.0f;
	float ShakingScreenValue = 0.0f;
	bool ShakingScreenFlag = true;
	XMFLOAT3 OldEyeRot;

	float easetimer = 0;

	bool MouseStopFlag = false;
	float ShakeLimitTime = 0;
	bool ShakingStartFlag = false;
	XMVECTOR OldPos;
	bool RecoilGunFlag = false;
	float RecoveryTime = 0.0f;
	//持ってきたもの
	int Remaining = 0;
	int OldRemaining = 0;

	XMFLOAT2 SpriteSiz = { 64.0f,64.0f };
	XMFLOAT2 SpritePos[9];
	bool DropBulletFlag[9];
	float SpriteRot[9];
	float Gravity = 9.8f;
	float Time[9];


	//Spriteのステータス
	XMFLOAT4 ReloadSpriteColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 AnchorPoint = { 0.5f,0.5f };
	XMFLOAT2 ReloadSpritePos = { 1070,310 };
	XMFLOAT2 ReloadSpriteSize = { 160,90 };

	bool ReversFlag = false;
};

