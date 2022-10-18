#pragma once
#include"BaseScene.h"
#include "DirectXCommon.h"
#include"Sprite.h"
#include <DirectXMath.h>
#include<memory>

class GameOverScene :public BaseScene
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
	GameOverScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Update() override;

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:
	Sprite* GameOver = nullptr;
	XMFLOAT2 size = { 1280.0f,720.0f };
	XMFLOAT2 pos = { 0.0f,0.0f };
};

