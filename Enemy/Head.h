#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include<memory>
class Head
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void SetPRS(XMVECTOR bodyPos);

	void Update(bool arive,XMVECTOR bodyPos);

	void Draw(bool arive);

	void Finalize();
private:
	std::unique_ptr<Object3d> Head;
	ObjModel* headModel = nullptr;

	XMVECTOR HeadPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 HeadScl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 HeadRot = { 0.0f,0.0f,0.0f };
};

