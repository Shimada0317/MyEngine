#pragma once

#include"GameScene.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Input.h"

/// <summary>
/// フレームワーク
/// </summary>
class Framework
{
public:
	bool GetFinish() { return finish; }
private:
	bool finish = false;

	DirectXCommon* dxCommon = nullptr;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	GameScene* gamescene = nullptr;
};

