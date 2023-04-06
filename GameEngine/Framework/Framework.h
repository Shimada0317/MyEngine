#pragma once

#include"GameScene.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"Input.h"
#include"Audio.h"
#include"Object3d.h"
#include"DebugText.h"
#include"SceneManager.h"
#include"DebugCamera.h"
#include"PostEffect.h"
#include"ParticleManager.h"
#include"Texture.h"
#include"Light.h"
#include"Mouth.h"

/// <summary>
/// フレームワーク
/// </summary>
class Framework
{
public:
	/// <summary>
	/// 実行
	/// </summary>
	void Run();

	void End();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
		/// 更新
		/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	bool GetFinish() { return finish; }
private:
	static const int debugTextNumber = 0;
protected:
	bool finish = false;
	DebugText* debugText;
	DirectXCommon* dxCommon = nullptr;
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	Mouth* mouse = nullptr;
	GameScene* gamescene = nullptr;
	Audio* audio = nullptr;
	SceneManager* sceneManager_ = nullptr;
	DebugCamera* camera = nullptr;
	PostEffect* postEffect = nullptr;
	Texture* texture = nullptr;
};

