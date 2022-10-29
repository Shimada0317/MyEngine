#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Bullet.h"
#include"Collision.h"

#include<memory>

class Body
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	~Body();

	void Initialize();

	void SetPRS(const XMVECTOR& allPos,Bullet* bull);

	void Update(bool& arive,const XMVECTOR& allPos,Bullet* bull,int& Hp);

	void Draw(bool arive);

	void Finalize();
private:
	std::unique_ptr<Object3d> body;
	ObjModel* bodyModel = nullptr;

	XMVECTOR BodyPos = {0.0f,0.0f,0.1f};
	XMFLOAT3 BodyScl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 BodyRot = { 0.0f,90.0f,0.0f };

	XMVECTOR BullPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 BullScl = { 0.0f,0.0f,0.0f };

	bool Hit = false;
};

