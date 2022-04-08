#pragma once

#pragma once

#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Input.h"
#include "Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include"Audio.h"
#include"DebugText.h"

class TitleScene
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

	void Initialize(DirectXCommon* dxCommon);

	void TitleSceneInitialize();

	void Update(DirectXCommon* dxCommon, Input* input);

	void Draw(DirectXCommon* dxCommon);

	void Finalize();
private:
	Sprite* sprite = nullptr;

	Sprite* title = nullptr;

	DirectXCommon* dxCommon = nullptr;

	Object3d* player3d = nullptr;
	Model* playermodel = nullptr;

	Object3d* ramieru3d = nullptr;
	Model* ramieru = nullptr;

	Object3d* human3d = nullptr;
	Model* human = nullptr;


	DebugText* debugText;
};

