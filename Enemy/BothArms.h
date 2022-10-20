#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Bullet.h"
#include<memory>

class BothArms
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	~BothArms();

	void Initialize();

	void SetPRS(const XMVECTOR& bodyPos, Bullet* bull);

	void Update(bool& arive, const XMVECTOR& bodyPos, Bullet* bull, int& Hp);

	void Draw(bool arive);

	void Finalize();

	void Attack(float& attackT,bool& AttackFase,int& playerHp);
private:
	std::unique_ptr<Object3d> LArm;
	std::unique_ptr<Object3d> RArm;
	ObjModel* ArmModel = nullptr;

	XMVECTOR LArmPos = { 0.0f,0.0f,0.0f };
	XMVECTOR RArmPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ArmScl = { 0.2f,0.2f,0.2f };
	XMFLOAT3 ArmRot = { 90.0f,0.0f,0.0f };

	XMVECTOR bullPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bullScl = { 0.0f,0.0f,0.0f };

	bool attack = false;
	int HitCount = 0;
	bool Hit = false;
};

