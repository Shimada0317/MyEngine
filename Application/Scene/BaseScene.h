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
//�O���錾
class SceneManager;

/// <summary>
/// �V�[�����
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
	//�V�[���}�l�[�W���[(�؂�Ă���)
	SceneManager* sceneManager_ = nullptr;
};

