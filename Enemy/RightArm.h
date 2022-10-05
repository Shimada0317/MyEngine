#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Bullet.h"
#include<memory>
class RightArm
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void SetPRS(XMVECTOR bodyPos,Bullet* bull);

	void Update(bool& arive, XMVECTOR bodyPos,Bullet* bull,int& Hp);

	void Draw(bool arive);

	void Finalize();

	void Attack(float attackT);
private:
	std::unique_ptr<Object3d> RArm;
	ObjModel* RArmModel = nullptr;

	XMVECTOR RArmPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 RArmScl = { 0.3f,0.4f,0.3f };
	XMFLOAT3 RArmRot = { 90.0f,0.0f,0.0f };

	XMVECTOR bullPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bullScl = { 0.0f,0.0f,0.0f };

	bool attack = false;
	int HitCount = 0;
	bool Hit = false;

};

