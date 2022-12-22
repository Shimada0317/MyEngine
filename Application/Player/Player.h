#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"
#include"Texture.h"
#include"TextureModel.h"
#include"WinApp.h"
#include<vector>
#include"Bullet.h"
#include"ParticleManager.h"
#include"RailCamera.h"
#include"Mouse.h"

#include<memory>
#include<list>

const int BULL = 9;

class Player
{
public:
	~Player();

	void Initalize();

	void Set();

	void Updata(Bullet* bull[], int& Remaining);

	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);

	void SpriteDraw();

	void CameraWork();

	void PlayerMove(bool& move,int patern);

	void ObjDraw();

	void ImGuiDraw();

	void Finalize();

	void ChangeViewPort(XMMATRIX& mat);

	void ReteicleHaiti();

	void MouthContoroll();

#pragma region Get
	const bool& GetFinish() { return Finish; }
	//三次元座標
	const XMVECTOR& GetPosition() { return position; }
	//角度
	const XMFLOAT3& GetRotation() { return rotation; }
	//スケール
	const XMFLOAT3& GetScl() { return scale; }
	//行列
	const XMMATRIX& GetMat() { return mat; }
	//二次元座標
	const XMFLOAT2& GetRetPosition() { return retpos; }
	//二次元スケール
	const XMFLOAT2& GetRetSiz() { return retsize; }

	const bool& GetCamWork() { return CamWork; }

	const bool& GetFring() { return Fring; }
	//Hp
	int GetHp() { return Hp; }
#pragma endregion

#pragma region Set

	void SetPosition(const XMVECTOR& position) { this->position = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->scale = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->retpos = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->retsize = scale; }
	void SetHp(int HP) { this->Hp = HP; }
	void SetFinish(const bool& finish) { this->Finish = finish; }
#pragma endregion
private:

	//プレイヤーオブジェクトとモデル
	std::unique_ptr<Object3d> Track;
	ObjModel* TrackModel = nullptr;

	std::unique_ptr<Sprite> spriteRet;
	std::unique_ptr<Sprite> curtain;
	std::unique_ptr<Sprite> curtain2;
	std::unique_ptr<Sprite> window;
	std::unique_ptr<Sprite> skip;

	std::unique_ptr<Object3d> gun;
	ObjModel* gunModel = nullptr;

	std::unique_ptr<Object3d> player;
	ObjModel* playerModel = nullptr;

	std::unique_ptr<Object3d> elevetor;
	ObjModel* eleModel = nullptr;

	//発砲時のエフェクト
	ParticleManager* part;

	Mouse* mouse = nullptr;
	Camera* camera = nullptr;
	RailCamera* cam;
	//ローカル
		//レティクルObj
		XMVECTOR position = { 0.0f,55.0f,0.0f };
		XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
		XMFLOAT3 scale = { 0.3f,0.3f,0.3f };
		//発射台Obj
		XMVECTOR gunPos = { 0.0f,0.0f,-6.0f };
		XMFLOAT3 gunRot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 gunScal={0.3f,0.3f,0.3f};
		//プレイヤーObj
		XMVECTOR playerPos = { 0.0f,0.0f,0.0f };
		XMFLOAT3 playerRot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 playerScl = { 0.3f,0.3f,0.3f };
	//ワールド
		//レティクル
		XMVECTOR TrackWorldPos = { 0.0f,0.0f,0.0f };
		XMMATRIX mat;
		//発射台
		XMVECTOR gunWorldPos = { 0.0f,0.0f,-5.0f };
		XMMATRIX gunmat;
		//プレイヤー
		XMVECTOR playerWorldPos = { 0.0f,0.0f,-0.1f };
		XMMATRIX playermat;
	//ビューポート
	XMMATRIX matViewPort;
	//カメラ
	XMFLOAT3 Eye_rot = {0.0f,0.0f,0.0f};
	XMFLOAT3 Target_pos = {0.0f,0.0f,0.0f};
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };
	//スプライト
	XMFLOAT4 spCol = { 1,1,1,1 };
	XMFLOAT2 anc = { 0.5f,0.5f };
	XMFLOAT2 retpos = { 640.0f,360.0f };
	XMFLOAT2 retsize = { 64.0f,64.0f };

	XMFLOAT2 curtainPos = { 0.0f,-100.0f };
	XMFLOAT2 curtainSiz = { 1280.0f,100.0f };

	XMFLOAT2 curtainPos2 = { 0.0f,720.0f };

	XMFLOAT2 windowPos = { 0.0f,0.0f };
	XMFLOAT2 skipPos = { 1100,720.0f };

	XMVECTOR offset = { 0,0,1.0f };

	
	float time = 0.0f;
	bool particle = false;
	int Hp = 5;
	//Reload
	bool ReloadFlag = false;
	int ReloadTime = 0;
	int ans = 0;

	XMVECTOR vel;
	bool Active = false;
	int ver = 0;
	bool chan = false;
	float kBulletSpeed=0;
	XMVECTOR velocity = { 0.0f,0.0f,0.0f };
	//動いたときのtimer
	float movetimer = 0.0f;
	int waveCount = 0;

	//cam
	int shake = 0;

	bool Finish = false;

	bool CamWork = false;

	bool a = false;
	bool stanby = false;
	int act = 0;
	float actTime = 0.0f;

	bool start = false;

	bool Fring = false;

	bool Shake = true;
};

