#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Bullet.h"
#include"Collision.h"
#include"ParticleManager.h"
#include"Camera.h"

#include<memory>
class Head
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	~Head();

	void Initialize(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation,Camera* camera);

	void SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation,Bullet* bull);

	void Update(bool& arive,const XMVECTOR& bodyPos, const XMFLOAT3 rotation,Bullet* bull,int& Hp);

	void Draw(bool arive);

	void Motion(const float& rot);

	void Finalize();

	const XMFLOAT3& GetScl() { return Head_Scl; }

	void SetScl(XMFLOAT3 scl) { this->Head_Scl = scl; }
private:
	std::unique_ptr<Object3d> Head;
	ObjModel* headModel = nullptr;

	std::unique_ptr<Bullet>bull;
	//頭のステータス
	XMVECTOR Head_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Head_Scl = { 0.3f,0.3f,0.3f };
	XMFLOAT3 Head_Rot = { 0.0f,0.0f,0.0f };
	XMMATRIX BullMat;
	//弾のステータス
	XMVECTOR Bull_Pos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Bull_Scl = { 0.0f,0.0f,0.0f };
	XMFLOAT4 HitColor = { 1,1,1,1 };
	XMMATRIX HeadMat;

	bool Hit_F = false;
	bool RotationHead_F = true;
	float MotionRand = 0;
};

