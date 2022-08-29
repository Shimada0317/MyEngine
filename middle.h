#pragma once
#include"Action.h"
#include"Player.h"
#include"Bullet.h"
#include"Enemy.h"
#include"Input.h"
#include"Collision.h"
#include"BaseScene.h"
#include"ClearScene.h"

#include<memory>

using namespace std;

class middle
{
public: 
	void Initialize();

	void SetPSR();

	void AllUpdate();

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SpriteDraw();

	void ImGuiDraw();

	void Fainalize();
public:
	void SetPatern(int patern) { this->patern = patern; }

	const int& GetPatern() { return patern; }
private:
	unique_ptr <Player> player;
	unique_ptr <Bullet> bull[9];
	unique_ptr <Enemy> enemy[2];

	Sprite* bulletHUD[9];
	Sprite* Reload = nullptr;
	Sprite* wave = nullptr;
	Sprite* maxcount = nullptr;
	Sprite* slash = nullptr;
	Sprite* changecount[5];
	//プレイヤーのステータス
	XMFLOAT3 playerPos = { 0,0,0 };
	XMFLOAT3 playerScl = { 0,0,0 };
	XMFLOAT3 playerRot = { 0,0 ,0 };
	//弾のステータス
	XMFLOAT3 bullPos[9];
	XMFLOAT3 bullScl = { 0,0,0 };
	XMFLOAT3 bullRot = { 0,0,0 };
	//敵のステータス
	XMFLOAT3 enemyPos[2];
	XMFLOAT3 enemyScl = { 0,0,0 };
	XMFLOAT3 enemyRot = { 0,0,0 };

	XMFLOAT3 absolutePos = { 0,0,0 };

	XMFLOAT2 spSiz = { 64.0f,64.0f };
	XMFLOAT2 spPos = { 1200.0f,0.0f };

	//弾の発射地点
	XMFLOAT3 startPos = { 2,-0.1f,-1 };

	Input* input = nullptr;

	bool arive = false;

	float mouseX = 0;
	float mouseY = 0;

	float moveSpeed = 0.1f;
	bool shot [9];
	int debug[9];
	float speed[9];
	int next = 0;
	bool retime[9];
	bool reshot = false;

	bool lost = false;
	int life[2];
	//リロード機能
	int Remaining = 0;
	bool ReloadFlag = false;
	int ReloadTime = 0;
	int ans = 0;

	float speedm = 0.1f;

	XMMATRIX matVPV;
	XMMATRIX matInverseVPV;
	
	XMFLOAT3 eye = { 0, 0, -5 };
	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 up = { 0,1,0 };

	float enespeed[2];
	float stopT = 0;
	bool stop[2];

	float hit = 0;
	int patern = 0;
	float cammove = 0.1f;
	bool count = false;
	//プレイヤーの向きに発射
	XMVECTOR verosity_;
	const float bullSpeed = 0.1f;
};

