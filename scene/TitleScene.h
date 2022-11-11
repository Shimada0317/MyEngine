#pragma once

#pragma once

#include"BaseScene.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"ObjModel.h"
#include"Input.h"
#include"ParticleManager.h"
#include"Player.h"
#include"Bullet.h"
#include"middle.h"
#include"PostEffect.h"
#include"Robot.h"
#include"PostEffect.h"
#include<memory>

class TitleScene :public BaseScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	static const int debugTextNumber = 0;

public://メンバ変数
	TitleScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Updata() override;

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:

	DirectXCommon* dxCommon = nullptr;

	Sprite* title = nullptr;

	PostEffect* post = nullptr;

	Player* pl = nullptr;

	XMFLOAT3 ramieru_pos = { 0,0,0 };

	XMFLOAT2 titlesize = { 1280.0f,720.0f };
	XMFLOAT2 titlepos = { 0.0f,0.0f };
	POINT pos;

	int timer = 0;
};

