#pragma once
#include"DirectXCommon.h"


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

	virtual void Updata() = 0;

	virtual void Draw(DirectXCommon* dxCommon) = 0;
protected:
	//シーンマネージャー(借りてくる)
	SceneManager* sceneManager_ = nullptr;
};

