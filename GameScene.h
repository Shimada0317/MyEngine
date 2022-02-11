#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include"Audio.h"
#include"DebugText.h"

class GameScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	static const int debugTextNumber = 0;

public://メンバ変数

	void Initialize(DirectXCommon* dxCommon);

	void GameSceneInitialize();

	void Update(DirectXCommon* dxCommon,Input* input);

	void Draw(DirectXCommon* dxCommon);
private:

	const float WindowWidth = 1280;
	const float WindowHeight = 720;

	Sprite* sprite = nullptr;

	Sprite* spritetest = nullptr;
	Sprite* life = nullptr;
	Sprite* title = nullptr;
	Sprite* clear = nullptr;
	Sprite* gameover = nullptr;
	Sprite* last = nullptr;
	Input* input = nullptr;
	
	

	DirectXCommon* dxCommon = nullptr;

	Object3d* block3d[20] = { nullptr };
	Object3d* player3d = nullptr;
	Object3d* player3d1 = nullptr;
	Object3d* player3d2 = nullptr;

	Model* model = nullptr;
	Model* playermodel = nullptr;
	Model* playermodel2 = nullptr;
	Model* playermodel3 = nullptr;

	DebugText* debugText;

	XMFLOAT3 rotation = { 0,450,-270 };
	XMFLOAT3 position = { 0,0,0 };
	XMFLOAT3 speed = { 0,0,0 };
	XMFLOAT3 scale = { 0.5f,0.5f,0.5f };
	XMFLOAT2 size = { 50,50 };
	XMFLOAT3 enemyspeed[20];

	int scene = 0;
	//当たり判定
	float playerradius = 0.5f;
	float enemyradius[20];
	float a[20];
	float b[20];
	float c[20];
	float d[20];
	int checker = 0;
	int background = 0;
	//敵生存フラグ
	int enemyF[20];
	int count = 0;

	//動きのタイマー
	int movetimer = 0;
	int moveAction = 0;
	int move = 0;

	//音量調節
	float vol[2];

};

