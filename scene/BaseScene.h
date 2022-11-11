#pragma once
#include"DirectXCommon.h"


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

	virtual void Updata() = 0;

	virtual void Draw(DirectXCommon* dxCommon) = 0;
protected:
	//�V�[���}�l�[�W���[(�؂�Ă���)
	SceneManager* sceneManager_ = nullptr;
};

