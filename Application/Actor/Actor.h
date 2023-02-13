#pragma once
#include"Audio.h"
#include"Player.h"
#include"BaseScene.h"
#include"ClearScene.h"
#include<sstream>
#include"Enemy.h"

#include<memory>

using namespace std;

class Actor
{
public: 
	~Actor();

	void Initialize();

	void SetPSR();

	void Update();

	void Draw(DirectXCommon* dxCommon);

	void SpriteDraw();

	void ImGuiDraw();

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
	//ヘリObj
	unique_ptr<Object3d> heri;
	unique_ptr<Object3d> Goal;
	unique_ptr<Object3d> hane;

	
	Sprite* bulletHUD[9];
	Sprite* Reload = nullptr;
	Sprite* wave = nullptr;
	Sprite* maxcount = nullptr;
	Sprite* slash = nullptr;
	Sprite* changecount[5];
	Sprite* LifeCount[5];
	Sprite* HpBer;

	Audio* heriFry = nullptr;
	//プレイヤーのステータス
	XMVECTOR playerPos = { 0,0,0 };
	XMVECTOR backplayer = { 0,0,-5 };
	XMFLOAT3 playerScl = { 0,0,0 };
	XMFLOAT3 playerRot = { 0,0 ,0 };
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

	//敵の変数
	int next = 0;
	bool retime[9];
	bool reshot = false;

	bool lost = false;
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

	float stopT = 0;


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


	//DebugSceneのやつを持ってきた
	std::list<std::unique_ptr<Enemy>>Robot;
	std::unique_ptr<Player>player;


	int playerHp;

	int Remaining = 0;

	bool StartMovie = false;

	bool finish = false;

	bool GetCamWork_F = false;

	Camera* camera;

	//Spriteのステータス
	XMFLOAT4 ReloadSpriteColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 AnchorPoint = { 0.5f,0.5f };
	XMFLOAT2 ReloadSpritePos = { 1070,310 };
	XMFLOAT2 ReloadSpriteSize = { 160,90 };

	bool Revers = false;
};

