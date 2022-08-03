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

	void ImGuiDraw();

	void Fainalize();
private:
	Player* player = nullptr;
	Bullet* bull = nullptr;
	Enemy* enemy[2];

	XMFLOAT3 playerPos = { 0,0,0 };
	XMFLOAT3 playerScl = { 0,0,0 };
	XMFLOAT3 playerRot = { 0,0 ,0 };

	XMFLOAT3 bullPos = { 0,0,0 };
	XMFLOAT3 bullScl = { 0,0,0 };
	XMFLOAT3 bullRot = { 0,0,0 };

	XMFLOAT3 enemyPos[2];
	XMFLOAT3 enemyScl = { 0,0,0 };
	XMFLOAT3 enemyRot = { 0,0,0 };

	XMFLOAT3 absolutePos = { 0,0,0 };

	Input* input = nullptr;

	bool arive = false;

	float mouseX = 0;
	float mouseY = 0;

	float moveSpeed = 0.1f;
	bool shot = false;
	float speed = 0.5f;

	bool lost = false;
	int life[2];

	int Remaining = 9;

	XMMATRIX matVPV;
	XMMATRIX matInverseVPV;
	
	XMFLOAT3 eye = { 0, 0, -5 };
	XMFLOAT3 target = { 0,0,0 };
	XMFLOAT3 up = { 0,1,0 };

	float enespeed = 0.1f;
	float stopT = 0;
};

