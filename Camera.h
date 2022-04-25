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

	virtual ~Camera();

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

/// <summary>
/// �r���[�s��
/// </summary>
	inline const XMMATRIX& GetViewMatrix() {
		return matView;
	}

/// <summary>
/// �ˉe�s��
/// </summary>
	inline const XMMATRIX& GetProjectionMatrix() {
		return matProjection;
	}


	/// <summary>
	/// �r���{�[�h�s��
	/// </summary>
	inline const XMMATRIX& GetBilbordMatrix() {
		return matBillbord;
	}
/// <summary>
/// ���_���W
/// </summary>
	inline const XMFLOAT3& GetEye() {
		return eye;
	}

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <param name="eye"></param>
	inline void SetEye(XMFLOAT3 eye) {
		this->eye = eye; viewflag = true;
	}

	/// <summary>
	/// �����_���W
	/// </summary>
	/// <returns></returns>
	inline const XMFLOAT3& GetTarget() {
		return target;
	}

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <param name="target"></param>
	inline void SetTarget(XMFLOAT3 target) {
		this->target = target; viewflag = true;
	}

	/// <summary>
	/// ������̃x�N�g��
	/// </summary>
	/// <returns></returns>
	inline const XMFLOAT3& GetUp() {
		return up;
	}

	/// <summary>
	/// ������̃x�N�g���擾
	/// </summary>
	/// <param name="up"></param>
	inline void SetUp(XMFLOAT3 up) {
		this->up = up;
	}
	
	void MoveEyeVectror(const XMFLOAT3& move);
	void MoveEyeVector(const XMVECTOR& move);


	void MoveVector(const XMFLOAT3& move);
	void MoveVector(const XMVECTOR& move);

private:
	//�r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	//�ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	//�r���{�[�h
	XMMATRIX matBillbord = DirectX::XMMatrixIdentity();
	//Y�����̃r���{�[�h�s��
	XMMATRIX matBillbordY = DirectX::XMMatrixIdentity();
	//�r���[�Ǝˉe�̍���
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	//�r���[�s��̃t���O
	bool viewflag = false;
	//�ˉe�s��̃t���O
	bool projectionflag = false;
	//���_���W
	XMFLOAT3 eye = { 0,0,-20 };
	//�����_���W
	XMFLOAT3 target = { 0,0,0 };
	//������x�N�g��
	XMFLOAT3 up = { 0,1,0 };
	//�A�X��
	float aspect = 1.0f;

};

