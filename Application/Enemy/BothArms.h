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

	void SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull);

	void Updata(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull, int& Hp);

	void Draw(bool arive);

	void Finalize();

	void RespownSet(const XMFLOAT3& rot);

	void Attack(float& attackT,bool& AttackFase,int& playerHp,const bool& arive);

	const XMFLOAT3& GetScl() { return ArmScl; }

	void SetScl(XMFLOAT3 scl) { this->ArmScl = scl; }
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
	XMFLOAT4 Color = { 1.0f,1.0f,1.0f,1.0f };



	bool attack = false;
	int HitCount = 0;
	bool Hit = false;
};