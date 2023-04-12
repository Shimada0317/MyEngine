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
/// �t���[�����[�N
/// </summary>
class Framework
{
public:
	/// <summary>
	/// ���s
	/// </summary>
	void Run();

	void End();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize();

	/// <summary>
		/// �X�V
		/// </summary>
	virtual void Update();
	/// <summary>
	/// �`��
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

