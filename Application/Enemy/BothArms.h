#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"ParticleManager.h"
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

	void Update(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull, int& Hp);

	void Draw(bool arive);

	void RespownSet(const XMFLOAT3& rot);

	void Attack(float& attackT,bool& AttackFase,int& playerHp,const bool& arive);

	const XMFLOAT3& GetScl() { return Arm_Scl; }

	void SetScl(XMFLOAT3 scl) { this->Arm_Scl = scl; }
private:
	std::unique_ptr<Object3d> Arm;
	ObjModel* ArmModel = nullptr;

	XMVECTOR Arm_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Arm_Scl = { 0.2f,0.2f,0.2f };
	XMFLOAT3 Arm_Rot = { 90.0f,0.0f,0.0f };
	XMFLOAT4 Color = { 1.0f,1.0f,1.0f,1.0f };

	bool Attack_F = false;
	bool Hit_F = false;
};