#pragma once

#include<DirectXMath.h>

struct Sphere
{
	//���S���W�n
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 1.0f;
};

struct Plane
{
	//�@���x�N�g��
	DirectX::XMVECTOR normal = { 0,0,0,0 };
	//���_(0,0,0)����̋���
	float distance = 0.0f;
};

struct Ray
{
	DirectX::XMVECTOR start = { 0,0,0,1 };
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

///<summary>
///�@���t���O�p�`(���v��肪�\��)
///</summary>
class Triangle
{
public:
	DirectX::XMVECTOR p0 = { -0.5f,-0.5f,0.0f };
	DirectX::XMVECTOR p1 = { 0.5f,0.5f,0.0 };
	DirectX::XMVECTOR p2 = { 0.5f,-0.5f,0.0 };
	//�@���x�N�g��
	DirectX::XMVECTOR normal;

	void ComputeNormal();
};




