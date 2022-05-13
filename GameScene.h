#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include"Input.h"
#include"BaseScene.h"
#include"FbxObject3d.h"
#include"DebugCamera.h"

class GameScene:public BaseScene
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
	

public://メンバ変数

	GameScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Update() override;

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:

	DirectXCommon* dxCommon = nullptr;

	Model* playermodel = nullptr;
	Model* ramieru = nullptr;
	Model* human = nullptr;

	Object3d* player3d = nullptr;
	Object3d* ramieru3d = nullptr;
	Object3d* human3d = nullptr;

	FbxModel* model = nullptr;
	FbxObject3d* Object = nullptr;

	Sprite* sprite = nullptr;
	Sprite* title = nullptr;

	DebugCamera* camera = nullptr;

	XMFLOAT3 ramieru_pos = { 0,0,0 };

	
};

