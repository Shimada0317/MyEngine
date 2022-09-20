#pragma once
#include<DirectXMath.h>
#include"Object3d.h"

class Body
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void SetPRS();

	void Update(bool arive);

	void Draw(bool arive);
private:
	Object3d* body = nullptr;

	XMVECTOR BodyPos = {0.0f,0.0f,0.0f};
	XMFLOAT3 BodyScl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 BodyRot = { 0.0f,0.0f,0.0f };
};

