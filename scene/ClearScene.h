#pragma once
#include"BaseScene.h"
#include "DirectXCommon.h"
#include"Sprite.h"
#include <DirectXMath.h>
#include<memory>


class ClearScene:public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	static const int debugTextNumber = 0;

public://�����o�ϐ�
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

