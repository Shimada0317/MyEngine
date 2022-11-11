#pragma once
#include"BaseScene.h"
#include "DirectXCommon.h"
#include"Sprite.h"
#include <DirectXMath.h>
#include<memory>


class ClearScene:public BaseScene
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
	ClearScene(SceneManager* sceneManager_);

	void Initialize(DirectXCommon* dxComon) override;

	void SetPosSclRot();

	void Updata() override;

	void Draw(DirectXCommon* dxCommon)override;

	void Finalize() override;
private:
	Sprite* Clear = nullptr;
	XMFLOAT2 retsize = { 1280.0f,720.0f };
	XMFLOAT2 retpos = { 0.0f,0.0f };
};

