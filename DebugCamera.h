#pragma once
#include"Camera.h"
#include"Input.h"

class DebugCamera:public Camera
{
	using XMMATRIX = DirectX::XMMATRIX;
public:

	DebugCamera(int window_width, int window_height, Input* input);

	//�X�V
	void Update();

	void SetDistance(float distance) {this->distance = distance;}

private:
	Input* input;
	//����
	float distance;
	//X,Y�X�P�[��
	float scaleX;
	float scaleY;
	//��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

};

