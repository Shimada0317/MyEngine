#pragma once
#include"Object3d.h"
#include"ObjModel.h"
#include"LeftArm.h"

class Wepon
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void SetPRS(LeftArm* LArm);

	void Update(LeftArm* LArm);

	void Attack();

	void Draw();

	void Finalize();
private:
	std::unique_ptr<Object3d>wepon;
	ObjModel* weponModel;

	XMVECTOR posiiton = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT3 Rotation = { 0.0f,0.0f,0.0f };

};

