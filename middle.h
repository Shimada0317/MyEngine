#pragma once
#include"Action.h"
#include"Player.h"
#include"Bullet.h"
#include"Enemy.h"
#include"Input.h"
#include"Collision.h"

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
private:
	Player* player = nullptr;
	Bullet* bull[9];
	Enemy* enemy[2];

	Sprite* bullet=nullptr;

	XMFLOAT3 playerPos = { 0,0,0 };
	XMFLOAT3 playerScl = { 0,0,0 };
	XMFLOAT3 playerRot = { 0,0 ,0 };

	XMFLOAT3 bullPos[9];
	XMFLOAT3 bullScl = { 0,0,0 };
	XMFLOAT3 bullRot = { 0,0,0 };

	XMFLOAT3 enemyPos[2];
	XMFLOAT3 enemyScl = { 0,0,0 };
	XMFLOAT3 enemyRot = { 0,0,0 };

	XMFLOAT3 absolutePos = { 0,0,0 };

	XMFLOAT2 spSiz = { 0.1f,0.1f };
	XMFLOAT2 spPos = { 1.0f,1.0f };

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

	int Remaining = 0;

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
};

