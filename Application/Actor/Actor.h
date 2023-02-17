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

	int GetHp() { return PlayerHp; }
private:
	//ヘリObj
	unique_ptr<Object3d> heri;
	unique_ptr<Object3d> Goal;
	unique_ptr<Object3d> hane;

	
	Sprite* bulletHUD[9];
	Sprite* Reload = nullptr;
	Sprite* Wave = nullptr;
	Sprite* MaxCount = nullptr;
	Sprite* Slash = nullptr;
	Sprite* ChangeCount[5];
	Sprite* LifeCount[5];
	Sprite* HpBer;

	Audio* heriFry = nullptr;
	//ヘリステータス
	XMVECTOR GoalPos = { 45.0f,3.0f,92.0f };
	XMFLOAT3 GoalScl = { 3.0f,3.0f,3.0f };

	XMVECTOR Heripos = { 0.0f,75.0f,-160.0f };
	XMFLOAT3 Heriscl = { 10.0f,10.0f,10.0f };


	float HeriY = 0.0f;
	bool Move = false;
	bool BackObj = true;
	float HeriX = 3.1f;

	XMFLOAT2 SpriteSiz = { 64.0f,64.0f };
	XMFLOAT2 SpritePos[9];
	bool DropBullet[9];
	float SpriteRot[9];

	int patern = 0;
	bool count = false;
	//敵発生コマンド
	std::stringstream EnemyPopCommands;
	//待機コマンド
	bool Wait_F = false;
	int WaitT = 0;


	//DebugSceneのやつを持ってきた
	std::list<std::unique_ptr<Enemy>>Robot;
	std::unique_ptr<Player>player;


	int PlayerHp;

	int Remaining = 0;
	int OldRemaining = 0;

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

