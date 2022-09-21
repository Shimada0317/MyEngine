#pragma once
#include"Body.h"
#include"RightArm.h"
#include"LeftArm.h"
#include"Head.h"
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

	void SetPRS();

	void AllUpdate();

	void Update();

	void Draw();

	void Finalize();
private:
	std::unique_ptr<Head> head;
	std::unique_ptr<RightArm> RArm;
	std::unique_ptr<LeftArm>LArm;
	std::unique_ptr<Body>body;

	XMVECTOR allPos = { 0.0f,0.0f,10.0f };

	bool arive = true;

};

