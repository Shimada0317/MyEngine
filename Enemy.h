#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"
#include<vector>

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

private:
	ObjModel* model = nullptr;
	Object3d* enemy = nullptr;

	Input* input = nullptr;
	DebugText* debugtext = nullptr;
	DebugCamera* camer = nullptr;

	XMFLOAT3 position = { 0.0f,0.0f,2.0f };
	XMFLOAT3 cameraEye = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 cameraTarget = { 0.0f,1.0f,0.0f };

	bool arive = true;
};

