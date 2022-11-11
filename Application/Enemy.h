#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"

#include<vector>
#include<memory>

using namespace DirectX;

class Enemy
{
public:

	void Initalize();

	void Set();

	void Updata(XMVECTOR pos, int wave,int oldwave, bool& StopT, XMVECTOR positionP,bool& spown);

	void Draw();

	void ImGuiDraw();

	void Finalize();

	void Active(bool& StopT,XMVECTOR position);
	const XMVECTOR& GetPosition() { return position; }

	void SetPosition(XMVECTOR position) { this->position = position; }

	const XMFLOAT3& GetRotation() { return rotation; }

	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	const XMFLOAT3& GetScl() { return scale; }

	void SetScl(XMFLOAT3 scale) { this->scale = scale; }

	bool GetArive() { return arive; }

	void SetArive(bool arive) { this->arive = arive; }

	int GetLife() { return life; }

	void SetLife(int life) { this->life = life; }

	const XMVECTOR& GetWorldPos();

	void SetSpeed(float speed) { this->speed = speed; }

	float GetSpeed() { return speed; }

	void SetTime(float time) { this->stopT = time; }

private:
	ObjModel* model = nullptr;
	std::unique_ptr<Object3d> enemy;



	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camer = nullptr;

	XMVECTOR position = { 0.0f,0.0f,10.0f };
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };

	XMFLOAT3 bulpos = { 0,0,0 };
	XMFLOAT3 bulscl = { 0,0,0 };

	bool arive = true;
	int life = 3;
	float responetime = 0.0f;
	float speed = 0.0f;
	float stopT = 0.0f;

	float timer = 0;
	int oldwave = 0;
	float nowpos = 0;
};

