#pragma once
#include"Body.h"
#include"RightArm.h"
#include"LeftArm.h"
#include"BothArms.h"
#include"Head.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include<DirectXMath.h>
#include<memory>
#include<list>


class Robot
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	~Robot();

	void Initialize();

	void SetPRS(const XMMATRIX& player);

	void AllUpdata(Bullet* bull);

	void Updata(Bullet* bull,bool& arive, const XMMATRIX& player,bool& spown,int& playerHp);

	void Draw(DirectXCommon* dxCommon);

	void ParticleDraw(DirectXCommon* dxCommon);

	void TrackPlayer();

	void Finalize();

	void SetPosition(XMVECTOR allPos) { this->allPos = allPos; }

	void SetRotation(XMFLOAT3 allRot) { this->allRot = allRot; }

	void SpownEnemy(const XMMATRIX& player,int patern);
private:
	std::unique_ptr<Head> head;
	/*std::unique_ptr<RightArm> RArm;
	std::unique_ptr<LeftArm>LArm;*/
	std::unique_ptr<BothArms>Arms;
	std::unique_ptr<Body>body;
	std::unique_ptr<ObjParticle>part;
	std::list<std::unique_ptr<ObjParticle>>particle_;


	XMVECTOR allPos = { 0.0f,0.0f,1.0f };
	XMFLOAT3 allRot = { 0.0f,0.0f,0.0f };

	bool arive[3];
	float attackT = 0.0f;
	float time = 0.0f;
	int Hp = 50;
	int OldHp = 0;

	XMFLOAT4 color = { 1,0,0,0.1f };

	XMVECTOR playerPos;

	int action = 0;

	bool oldArive;
	int AttackChanse = 10;
	bool AttackFase = false;
	float AttackTime = 0;
	float speed = 0.005f;
};

