#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include"Input.h"
#include"BaseScene.h"

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

	void Initialize() override;

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

	Sprite* sprite = nullptr;
	Sprite* title = nullptr;


	XMFLOAT3 ramieru_pos = { -15,0,10 };
	XMFLOAT3 ramieru_scl = { 0.5,0.5,0.5 };
	
	//重力
	float g = 0.01f;
	//タイム
	float t = 0;
	//横の速度
	float SpeedX=0.1f;
	//初速度
	float Start_Speed = 0.29f;
	//速度
	float Speed = 0;
	//フラグ
	int GravityF = 0;
	//摩擦
	float fricition = 0.01f;
};

