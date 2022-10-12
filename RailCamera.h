#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include"camera/Camera.h"

#include"Object3d.h"
#include"ObjModel.h"
#include<memory>

class RailCamera
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:

	~RailCamera();

	const XMFLOAT3& Getye() { return eye; }

	const XMFLOAT3& GetTarget() { return target; }

	const XMMATRIX& GetWorld() { return matWorld; }

	void Initialize(const XMVECTOR& Pos, const XMFLOAT3& Rot);

	void UpdateWorld();

	void Update(const XMVECTOR& vel={0.0f,0.0f,0.0f},const XMFLOAT3& rot={0.0f,0.0f,0.0f});

	void Draw();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

private:
	Camera* camera = nullptr;

	std::unique_ptr<Object3d> debug;
	ObjModel* debugModel;

	XMMATRIX world;

	// ���[���h�ϊ��s��
	XMMATRIX matWorld;

	XMMATRIX matRot, matTrans;

	XMVECTOR position;
	XMFLOAT3 rotation;

	// �r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��(�r���[�v���W�F�N�V����)
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// ������x�N�g��
	XMFLOAT3 up = { 0, 1, 0 };
	//�����_�@�@
	XMFLOAT3 eye = { 0.0f,0.0f,0.0f };

	XMFLOAT3 target = { 0.0f,0.0f,0.0f };

	// �A�X�y�N�g��
	float aspectRatio = 1.0f;

	XMVECTOR eyePosition;
	// �����_���W
	XMVECTOR targetPosition;
	// �i���́j�����
	XMVECTOR upVector;

};

