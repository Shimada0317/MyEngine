#pragma once
#include"Action.h"
#include"Player.h"
#include"Bullet.h"
#include"Enemy.h"
#include"Collision.h"

class middle
{
public: 
	void Initialize();

	void SetPSR();

	void AllUpdate();

	void Update();

	void Draw();

	void ImGuiDraw();

	void Fainalize();
private:
	Player* player = nullptr;
	Bullet* bull = nullptr;
	Enemy* enemy = nullptr;

	XMFLOAT3 playerPos = { 0,0,0 };
	XMFLOAT3 playerScl = { 0,0,0 };
	XMFLOAT3 playerRot = { 0,0,0 };

	XMFLOAT3 bullPos = { 0,0,0 };
	XMFLOAT3 bullScl = { 0,0,0 };
	XMFLOAT3 bullRot = { 0,0,0 };

	XMFLOAT3 enemyPos = { 0,0,0 };
	XMFLOAT3 enemyScl = { 0,0,0 };
	XMFLOAT3 enemyRot = { 0,0,0 };

	bool arive = true;

	float moveSpeed = 0.1f;
	bool shot = false;
};

