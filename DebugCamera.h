#pragma once
#include"Camera.h"
#include"Input.h"

class DebugCamera:public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	DebugCamera(int window_width, int window_height, Input* input);

	//更新
	void Update();

	void SetDistance(float distance) {this->distance = distance;}

private:
	Input* input;
	//距離
	float distance;
	//X,Yスケール
	float scaleX;
	float scaleY;
	//回転行列
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

};

