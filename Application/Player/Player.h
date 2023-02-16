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
	/// 更新処理
	/// </summary>
	/// <param name="bull">プレイヤーの弾</param>
	/// <param name="Remaining">残弾数</param>
	/// <param name="enePos">敵の座標</param>
	/// <param name="came">カメラ</param>
	/// <param name="Ene2dPos">敵の2D座標</param>
	/// <param name="pat">現在のフェイズ</param>
	void Update(int& Remaining, Camera* camera,int patern);

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
	//Objの描画
	void ObjDraw();
	//Imguiの描画
	void ImGuiDraw();
	//ビューポート行列の計算
	void ChangeViewPort(XMMATRIX& Track_Mat);
	//SE呼び出し
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
	//パーティクル発生
	void ParticleEfect();

#pragma region Get
	const bool& GetFinish() { return Finish; }
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
	const bool& GetCamWork() { return CameraWork_F; }

	const bool& GetFring() { return Fring_F; }
	//Hp
	int GetHp() { return Hp; }

	const bool& GetBulletShot() { return BulletShot_F; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->ReticlePos = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->ReticleRot = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->ReticleScl = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->ReticlePos2D = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->ReticleSize = scale; }
	void SetHp(int HP) { this->Hp = HP; }
	void SetFinish(const bool& finish) { this->Finish = finish; }
	void SetBulletShot(const bool& BulletShot_F) { this->BulletShot_F = BulletShot_F; }
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
	//発砲時のエフェクト
	ParticleManager* PartGreen = nullptr;
	ParticleManager* PartRed = nullptr;
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
		XMVECTOR GunPos = { 0.0f,1.0f,-12.0f };
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

	bool Particle_F = false;
	int Hp = 5;
	//Reload
	bool ReloadFlag = false;
	int ReloadTime = 0;
	int Anser = 0;
	//移動
	XMVECTOR vel;
	bool Move_F = false;
	//弾の速度
	float MoveSpeed=0;
	//動いたときのtimer
	float MoveTimer = 0.0f;
	int WaveCount = 0;

	//cam
	int shake = 0;

	bool Finish = false;

	bool CameraWork_F = false;

	bool MovieFlag = false;
	bool stanby = false;
	int ActionCount = 0;
	float ActionTimer = 0.0f;

	bool Start_F = false;

	bool Fring_F = false;

	bool ShakeHead_F = true;

	bool ReloadSound_F = true;

	float ChangeRot = 0;

	float Distance = 11;
	int EnemyCount = 0;

	int PaternCount = 0;

	const float  M_PI = 3.14;

	XMFLOAT3 pos;


	bool BulletShot_F = false;
	float ShotCoolTime = 0.0f;
	bool Recoil_F = false;
	float RecoilTime = 0.0f;
	float ShakingScreenValue = 0.0f;
	bool ShakingScreen_F = true;
	XMFLOAT3 OldEyeRot;

	float easetimer = 0;

	bool MouseStop_F = false;
	float ShakeLimitTime = 0;
	bool ShakingStart = false;
	XMVECTOR OldPos;
	bool RecoilGun = false;
	float RecoveryTime = 0.0f;
};

