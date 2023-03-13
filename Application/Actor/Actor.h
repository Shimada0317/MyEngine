#pragma once
#include"Audio.h"
#include"Player.h"
#include"BaseScene.h"
#include"ClearScene.h"
#include<sstream>
#include"Enemy.h"
#include"ThrowEnemy/ThrowEnemy.h"

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

	void CheckSameTrackPosition();
public:
	void SetPatern(int patern) { this->Patern = patern; }

	const int& GetPatern() { return Patern; }

	void SetGoalPos(const XMVECTOR& goalPos) { this->GoalPos = goalPos; }

	const XMVECTOR& GetGoalPos() { return GoalPos; }

	int GetHp() { return PlayerHp; }

	void SetVelocity(const XMVECTOR& Velocity) { this->Velo = Velocity; }

	const XMVECTOR& GetVelocity() { return Velo; }

private:
	//ヘリObj
	unique_ptr<Object3d> Heri;
	unique_ptr<Object3d> Goal;
	unique_ptr<Object3d> Hane;

	
	Sprite* Wave = nullptr;
	Sprite* MaxCount = nullptr;
	Sprite* ChangeCount[5];
	Sprite* LifeCount[5];
	Sprite* HpBer;
	

	Audio* heriFry = nullptr;
	//ヘリステータス
	XMVECTOR GoalPos = { 56.f,3.0f,92.0f };
	XMFLOAT3 GoalScl = { 3.0f,3.0f,3.0f };

	XMVECTOR Heripos = { 0.0f,75.0f,-160.0f };
	XMFLOAT3 Heriscl = { 10.0f,10.0f,10.0f };


	float HeriY = 0.0f;
	bool MoveFlag = false;
	bool BackObjFlag = true;
	float HeriX = 3.1f;

	

	int Patern = 0;
	bool CountFlag = false;
	//敵発生コマンド
	std::stringstream EnemyPopCommands;
	//待機コマンド
	bool WaitFlag = false;
	int WaitT = 0;


	//DebugSceneのやつを持ってきた
	std::list<std::unique_ptr<Enemy>>Robot;
	std::unique_ptr<Player>player;


	int PlayerHp;

	XMVECTOR Velo;

	bool StartMovieFlag = false;

	bool StopFlag = false;

	bool GetCamWorkFlag = false;


	Camera* camera;

	
};

