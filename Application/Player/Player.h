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

	void Initalize();

	void Set();

	void Effect();

	void Updata(Bullet* bull[], int& Remaining);

	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SpriteDraw();

	void PlayerMove(bool& move,int patern,bool& spown);

	void ObjDraw();

	void ImGuiDraw();

	void Finalize();

	void ChangeViewPort(XMMATRIX& mat);

	void Attack();

	void ReteicleHaiti();

	void MouthContoroll();

#pragma region Get
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
#pragma endregion
private:
	

	float OffsetX = 0.0f;
	float OffsetY = 0.0f;
	//プレイヤーオブジェクトとモデル
	std::unique_ptr<Object3d> player;
	ObjModel* playerModel = nullptr;

	std::unique_ptr<Sprite> spriteRet;

	std::unique_ptr<Object3d> gun;
	ObjModel* gunModel = nullptr;
	//発砲時のエフェクト
	ParticleManager* part;

	Input* input = nullptr;
	Mouse* mouse = nullptr;
	DebugText* debugtext = nullptr;
	Camera* camera = nullptr;
	RailCamera* cam;
	//ローカル
	XMVECTOR position = { 0.0f,0.0f,2.0f };
	XMVECTOR backPlayerPos{ 0.0f,0.0f,0.0f };
	XMVECTOR oldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 0.3f,0.3f,0.3f };
	XMVECTOR gunPos = { 0.0f,0.0f,-6.0f };
	XMFLOAT3 gunRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 gunScal={0.3f,0.3f,0.3f};
	//ワールド
	XMVECTOR playerWorldPos = { 0.0f,0.0f,0.0f };
	XMMATRIX mat;
	XMVECTOR gunWorldPos = { 0.0f,0.0f,-5.0f };
	XMMATRIX gunmat;
	//ビューポート
	XMMATRIX matViewPort;
	//カメラ
	XMFLOAT3 cameraTarget = { 0.0f,1.0f,0.0f };
	XMFLOAT3 camerapos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Eye_rot = {0.0f,0.0f,0.0f};
	XMFLOAT3 Target_pos = {0.0f,0.0f,0.0f};
	XMFLOAT3 up = { 0.0f,1.0f,0.0f };

	XMFLOAT4 color = { 1,1,1,1 };
	XMFLOAT4 spCol = { 1,1,1,1 };
	XMFLOAT2 anc = { 0.5f,0.5f };
	XMFLOAT2 retpos = { 640.0f,360.0f };
	XMFLOAT2 retsize = { 64.0f,64.0f };

	WinApp* winapp = nullptr;

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
	XMVECTOR veloc = { 0.0f,0.0f,0.0f };
	//動いたときのtimer
	float movetimer = 0.0f;
	int waveCount = 0;

	//cam
	int shake = 0;
};

