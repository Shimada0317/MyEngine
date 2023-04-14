#pragma once
#include"Audio.h"
#include"DirectXCommon.h"
#include"Light.h"
#include"LightGroup.h"
#include"Object3d.h"
#include"PostEffect.h"
#include"Sprite.h"
#include"ParticleManager.h"
#include"SpriteManager.h"
#include"Camera.h"

using namespace std;
//前方宣言
class SceneManager;

/// <summary>
/// シーン基底
/// </summary>
class BaseScene
{
public:
	BaseScene(SceneManager* sceneManager);

	virtual void Initialize(DirectXCommon* dxComon) = 0;

	virtual void Finalize() = 0;

	virtual void Update() = 0;

	virtual void Draw(DirectXCommon* dxCommon) = 0;

protected:
	//シーンマネージャー(借りてくる)
	SceneManager* sceneManager_ = nullptr;
};

