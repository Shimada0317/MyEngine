#pragma once
#include"Audio.h"
#include"Player.h"
#include"BaseScene.h"
#include"BossEnemy/BossEnemy.h"
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
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// ステータスセット
	/// </summary>
	void SetPSR();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);
	/// <summary>
	/// スプライト描画
	/// </summary>
	void SpriteDraw();
	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// csv読み込み
	/// </summary>
	void LoadEnemyPopData();
	/// <summary>
	/// 敵の生成
	/// </summary>
	void UpdataEnemyPopCommands();
	/// <summary>
	/// 敵同士の追尾先が被った時の確認とその場合の処理
	/// </summary>
	void CheckSameTrackPosition();
public:
	void SetPatern(int patern) { this->Patern = patern; }

	const int& GetPatern() { return Patern; }

	void SetGoalPos(const XMVECTOR& goalPos) { this->GoalPos = goalPos; }

	const XMVECTOR& GetGoalPos() { return GoalPos; }

	bool GetStop() { return StopFlag; }

	bool GetMove() { return MoveFlag; }

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
	Sprite* LifeCount[5];
	Sprite* HpBer;
	Sprite* hart;

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
	std::list<std::unique_ptr<BossEnemy>> Boss;
	std::list<std::unique_ptr<ThrowEnemy>>ThrowRobot;
	std::unique_ptr<Player>player;


	int PlayerHp;

	XMVECTOR Velo;

	bool StartMovieFlag = false;

	bool StopFlag = false;

	bool GetCamWorkFlag = false;


	Camera* camera;

	XMFLOAT2 HartSize = { 64,64 };
	XMFLOAT2 MotValue = { 0.f,0.f };
	bool ReversFlag = true;
	float EasingTimer = 0.0f;
	float AddTimer = 0.01f;
	
};

