#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"
#include<vector>
#include"Bullet.h"
#include"Collision.h"

using namespace DirectX;

class Enemy
{
public:

	void Initalize();

	void Set();

	void Update();

	void Draw();

	void ImGuiDraw();

	void Finalize();

	void Active();

	const XMFLOAT3& GetPosition() { return position; }

	void SetPosition(XMFLOAT3 position) { this->position = position; }


private:
	ObjModel* model = nullptr;
	Object3d* enemy = nullptr;

	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camer = nullptr;
	Bullet* bull = nullptr;

	XMFLOAT3 position = { 1.0f,1.0f,10.0f };
	XMFLOAT3 scale = { 1.0f,1.0f,1.0f };

	XMFLOAT3 bulpos = { 0,0,0 };
	XMFLOAT3 bulscl = { 0,0,0 };

	bool arive = true;
};

