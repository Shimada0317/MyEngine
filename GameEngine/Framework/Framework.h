#pragma once

#include"WinApp.h"
#include"DirectXCommon.h"
#include"Input.h"
#include"Audio.h"
#include"Object3d.h"
#include"SceneManager.h"
#include"DebugCamera.h"
#include"PostEffect.h"
#include"ParticleManager.h"
#include"Texture.h"
#include"Light.h"
#include"Mouse.h"
#include"SpriteManager.h"

using namespace std;

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
	unique_ptr <DirectXCommon> dxCommon = nullptr;
	unique_ptr< WinApp> winApp = nullptr;
	Input* input = nullptr;
	Mouse* mouse = nullptr;
	Audio* audio = nullptr;
	SceneManager* sceneManager_ = nullptr;
	unique_ptr<SpriteManager> spritemanager_ = nullptr;
	unique_ptr <DebugCamera> camera = nullptr;
	unique_ptr <PostEffect> postEffect = nullptr;
	unique_ptr <Texture> texture = nullptr;
};

