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
const int MAXENEMY = 10;

class middle
{
public: 
	~middle();

	void Initialize();

	void SetPSR();

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

	void SetGoalPos(const XMVECTOR& goalPos) { this->GoalPos = goalPos; }

	const XMVECTOR& GetGoalPos() { return GoalPos; }

	int GetHp() { return playerHp; }
private:
	
	//unique_ptr <Player> player;
	//unique_ptr <Bullet> bull[9];
	unique_ptr <Enemy> enemy[MAXENEMY];
	unique_ptr<Enemy>tst;
	//ヘリObj
	unique_ptr<Object3d> heri;
	unique_ptr<Object3d> Goal;
	unique_ptr<Object3d> hane;
	//ヘリモデル
	ObjModel* heriM;
	ObjModel* haneM;

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
	XMVECTOR enemyPos[5];
	XMFLOAT3 enemyScl = { 0,0,0 };
	XMFLOAT3 enemyRot[MAXENEMY];
	XMVECTOR absolutePos = { 0,0,0 };
	//ヘリステータス
	XMVECTOR GoalPos = { 45.0f,3.0f,92.0f };
	XMFLOAT3 GoalScl = { 3.0f,3.0f,3.0f };

	XMVECTOR heripos = { 0.0f,75.0f,-160.0f };
	XMFLOAT3 heriscl = { 10.0f,10.0f,10.0f };

	XMVECTOR start_pos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 start_scl = { 15.0f,15.0f,15.0f };


	float heriY = 0.0f;
	bool move = false;
	bool backObj = true;
	float heriX = 3.1f;
	float clearT = 0;

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
	int patern = 0;
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
	int waitT = 0;

	int ENE = 0;

	//DebugSceneのやつを持ってきた
	std::list<std::unique_ptr<Robot>>rob;
	std::unique_ptr<Body>bob;
	std::unique_ptr<Head>head;
	std::unique_ptr<Object3d> bo = nullptr;
	std::unique_ptr<Player>player;
	Bullet* bull[9];

	bool a = true;

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
	bool startmove = false;
	bool oldmove = false;
	bool spown = false;

	XMMATRIX playerMat;
	int rad;
	int oldrand[3];

	XMVECTOR TrackPoint[MAXENEMY];

	bool finish = false;

	bool start = false;

	bool getCamWorkF = false;

	int CountDistance=0;
};

