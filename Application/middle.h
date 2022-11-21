#pragma once
#include"Action.h"
#include"Player.h"
#include"Bullet.h"
#include"Enemy.h"
#include"Input.h"
#include"Collision.h"
#include"BaseScene.h"
#include"ClearScene.h"
#include"ObjParticle.h"
#include<sstream>
#include"Robot.h"

#include<memory>

using namespace std;
const int MAXENEMY = 5;

class middle
{
public: 
	~middle();

	void Initialize();

	void SetPSR();

	void AllUpdata();

	void Updata();

	void Draw(DirectXCommon* dxCommon);

	void SpriteDraw();

	void ImGuiDraw();

	void Fainalize();

	void SetEnemyPos();

	void Enemy2Enemy();

	void LoadEnemyPopData();

	void UpdataEnemyPopCommands();
public:
	void SetPatern(int patern) { this->patern = patern; }

	const int& GetPatern() { return patern; }

	int GetHp() { return playerHp; }
private:
	
	//unique_ptr <Player> player;
	//unique_ptr <Bullet> bull[9];
	unique_ptr <Enemy> enemy[MAXENEMY];

	unique_ptr<Enemy>tst;

	//]unique_ptr<Robot>rob;
	
	Sprite* bulletHUD[9];
	Sprite* Reload = nullptr;
	Sprite* wave = nullptr;
	Sprite* maxcount = nullptr;
	Sprite* slash = nullptr;
	Sprite* changecount[5];
	Sprite* LifeCount[5];
	Sprite* HpBer;
	//プレイヤーのステータス
	XMVECTOR playerPos = { 0,0,0 };
	XMVECTOR backplayer = { 0,0,-5 };
	XMFLOAT3 playerScl = { 0,0,0 };
	XMFLOAT3 playerRot = { 0,0 ,0 };
	//弾のステータス
	XMVECTOR bullPos[9];
	XMFLOAT3 bullScl = { 0,0,0 };
	XMFLOAT3 bullRot = { 0,0,0 };
	bool fire[9];
	float v3x=0;
	float v3y=0;
	float v3z=0;

	//敵のステータス
	XMVECTOR enemyPos[MAXENEMY];
	XMFLOAT3 enemyScl = { 0,0,0 };
	XMFLOAT3 enemyRot[MAXENEMY];
	XMVECTOR absolutePos = { 0,0,0 };


	XMFLOAT2 spSiz = { 64.0f,64.0f };
	XMFLOAT2 spPos = { 1200.0f,0.0f };

	//弾の発射地点
	XMFLOAT3 startPos = { 0,0.5f,-1 };

	Input* input = nullptr;
	bool moveTime = false;

	//bool arive = false;

	float mouseX = 0;
	float mouseY = 0;
	//敵の変数
	float moveSpeed = 0.1f;
	bool shot [9];
	int debug[9];
	float speed[9];
	int next = 0;
	bool retime[9];
	bool reshot = false;

	bool lost = false;
	int life[MAXENEMY];
	//リロード機能
	//int Remaining = 0;
	bool ReloadFlag = false;
	int ReloadTime = 0;
	int ans = 0;

	float speedm = 2.1f;

	XMMATRIX matInverseVPV;
	
	XMFLOAT3 eye = { 0, 0, -5 };
	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 up = { 0,1,0 };

	float enespeed[MAXENEMY];
	float stopT = 0;
	bool stop[MAXENEMY];

	float hit = 0;
	int patern = -1;
	int oldpatern = 0;
	float cammove = 0.1f;
	bool count = false;
	//プレイヤーの向きに発射
	XMVECTOR verosity_;
	const float bullSpeed = 1.0f;

	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//待機コマンド
	bool waitF = false;
	int waitTimer = 0;

	//DebugSceneのやつを持ってきた
	std::unique_ptr<Robot>rob[MAXENEMY];
	std::unique_ptr<Body>bob;
	std::unique_ptr<Head>head;
	std::unique_ptr<Object3d> bo = nullptr;
	std::unique_ptr<Player>player;
	Bullet* bull[9];

	ObjModel* bomodel = nullptr;

	XMVECTOR position = { 0.0f,0.0f,0.1f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 scale = { 0.1f,0.1f,0.1f };

	int playerHp;

	XMVECTOR allpos[3];

	DebugCamera* camera = nullptr;

	//XMVECTOR allpos = { 1.0f,1.0f,1.0f };
	XMFLOAT3 Eye_pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Target_pos = { 0.0f,0.0f,0.0f };
	bool arive = true;

	int Remaining = 0;

	bool all[MAXENEMY];
	bool waveCount = false;
	bool move = false;
	bool oldmove = false;
	bool spown = false;

	XMMATRIX playerMat;
	int rad;
	int oldrand[3];

	XMVECTOR TrackPoint[MAXENEMY];
};

