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

	void SetPRS(const XMVECTOR& allpos,const XMFLOAT3 rotation, Bullet* bull);

	void Update(bool& arive,const XMVECTOR& allpos, const XMFLOAT3 rotation, Bullet* bull,int& Hp);

	void Draw(bool arive);

	void Finalize();

	const XMVECTOR& GetPos() { return Body_Pos; }

	const XMFLOAT4& GetCol() { return HItColor; }

	const XMFLOAT3& GetScl() { return Body_Scl; }

	void SetScl(XMFLOAT3 scl) { this->Body_Scl = scl; }
private:
	std::unique_ptr<Object3d> Body;
	ObjModel* BodyModel = nullptr;

	XMVECTOR Body_Pos = {0.0f,0.0f,0.1f};
	XMFLOAT3 Body_Scl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 Body_Rot = { 0.0f,90.0f,0.0f };

	XMVECTOR BullPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 BullScl = { 0.0f,0.0f,0.0f };
	XMFLOAT4 HItColor = { 1,1,1,1 };


	XMMATRIX BodyMat;
	bool Hit = false;
};

