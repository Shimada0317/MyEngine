#pragma once
#include<DirectXMath.h>
#include"Object3d.h"
#include"ObjModel.h"
#include"Bullet.h"
#include"Collision.h"

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

	void Initialize();

	void SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation,Bullet* bull);

	void Updata(bool& arive,const XMVECTOR& bodyPos, const XMFLOAT3 rotation,Bullet* bull,int& Hp);

	void Draw(bool arive);

	void Finalize();
private:
	std::unique_ptr<Object3d> Head;
	ObjModel* headModel = nullptr;

	std::unique_ptr<Bullet>bull;

	XMVECTOR HeadPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 HeadScl = { 0.3f,0.3f,0.3f };
	XMFLOAT3 HeadRot = { 0.0f,0.0f,0.0f };

	XMVECTOR bullPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bullScl = { 0.0f,0.0f,0.0f };
	XMFLOAT4 HitColor = { 1,1,1,1 };

	bool Hit = false;
};

