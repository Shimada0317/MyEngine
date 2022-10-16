#pragma once
#include"Body.h"
#include"RightArm.h"
#include"LeftArm.h"
#include"Head.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include<DirectXMath.h>
#include<memory>


class Robot
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:

	void Initialize();

	void SetPRS(const XMMATRIX& player);

	void AllUpdate(Bullet* bull);

	void Update(Bullet* bull,bool& arive, const XMMATRIX& player,bool& spown);

	void Draw(DirectXCommon* dxCommon);

	void ParticleDraw(DirectXCommon* dxCommon);

	void Finalize();

	void SetPosition(XMVECTOR allPos) { this->allPos = allPos; }

	void SpownEnemy(const XMMATRIX& player);
private:
	std::unique_ptr<Head> head;
	std::unique_ptr<RightArm> RArm;
	std::unique_ptr<LeftArm>LArm;
	std::unique_ptr<Body>body;
	std::unique_ptr<ObjParticle>part;

	XMVECTOR allPos = { 0.0f,0.0f,1.0f };

	bool arive[4];
	float attackT = 0.0f;
	float time = 0.0f;
	int Hp = 50;
	int OldHp = 0;

	XMFLOAT4 color = { 1,0,0,0.1f };

	XMVECTOR playerPos;

	int action = 0;

	bool oldArive;

	float speed = 0.005f;
};

