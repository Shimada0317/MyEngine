#pragma once

#include"GameScene.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Input.h"

class MyGame
{
public:

	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	bool GetFinish() { return finish; }
private:
	bool finish = false;

	DirectXCommon* dxCommon = nullptr;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	GameScene* gamescene = nullptr;
};

