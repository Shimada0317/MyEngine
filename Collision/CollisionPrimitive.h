#pragma once

#include<DirectXMath.h>

struct Sphere
{
	//中心座標系
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.0f;
};

struct Plane
{
	//法線ベクトル
	DirectX::XMVECTOR normal = { 0,0,0,0 };
	//原点(0,0,0)からの距離
	float distance = 0.0f;
};

struct Ray
{
	DirectX::XMVECTOR start = { 0,0,0,1 };
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

///<summary>
///法線付き三角形(時計回りが表面)
///</summary>
class Triangle
{
public:
	DirectX::XMVECTOR p0 = { -0.5f,-0.5f,0.0f };
	DirectX::XMVECTOR p1 = { 0.5f,0.5f,0.0 };
	DirectX::XMVECTOR p2 = { 0.5f,-0.5f,0.0 };
	//法線ベクトル
	DirectX::XMVECTOR normal;

	void ComputeNormal();
};




