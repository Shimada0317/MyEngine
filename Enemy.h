#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"
#include<vector>

using namespace DirectX;

class Player;
class Enemy
{
public:

	void Initalize();

	void Set();

	void Update();

	void Draw();

	void ImGuiDraw();

	void Finalize();

	void Active(bool& StopT,int action,XMFLOAT3 position);
	const XMFLOAT3& GetPosition() { return position; }

	void SetPosition(XMFLOAT3 position) { this->position = position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	const XMFLOAT3& GetScl() { return scale; }

	void SetScl(XMFLOAT3 scale) { this->scale = scale; }

	const bool& GetArive() { return arive; }

	void SetArive(bool arive) { this->arive = arive; }

	const int& GetLife() { return life; }

	void SetLife(int life) { this->life = life; }

	void SetPlayer(Player* player) { player_ = player; }

	XMVECTOR GetWorldPos();

	void SetSpeed(float speed) { this->speed = speed; }

	const float& GetSpeed() { return speed; }

	void SetTime(float time) { this->stopT = time; }

private:
	ObjModel* model = nullptr;
	Object3d* enemy = nullptr;

	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camer = nullptr;

	XMFLOAT3 position = { 0.0f,0.0f,10.0f };
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };

	XMFLOAT3 bulpos = { 0,0,0 };
	XMFLOAT3 bulscl = { 0,0,0 };

	bool arive = true;
	int life = 3;
	float responetime = 0.0f;
	Player* player_ = nullptr;
	float speed = 0.0f;
	float stopT = 0.0f;

	float timer = 0;
};

