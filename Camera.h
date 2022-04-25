#pragma once
#include<DirectXMath.h>

class Camera
{
private://�G�C���A�X
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�����o�֐�

	Camera(int window_width, int window_height);

	void Iniitialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

/// <summary>
/// �r���[�s��̍X�V
/// </summary>
	void UpdatematView();

/// <summary>
/// �ˉe�s��̍X�V
/// </summary>
	void UpdatematProjection();

private:
	//�r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	//�r���{�[�h
	XMMATRIX matBillbord = DirectX::XMMatrixIdentity();
	//�ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	//���_���W
	XMFLOAT3 eye = { 0,0,-20 };
	//�����_���W
	XMFLOAT3 target = { 0,1,0 };
	//������x�N�g��
	XMFLOAT3 up = { 0,1,0 };

};

