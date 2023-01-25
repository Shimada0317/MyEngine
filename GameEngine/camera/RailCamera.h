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

	const XMFLOAT3& Getye() { return Eye; }

	const XMFLOAT3& GetTarget() { return Target; }

	const XMMATRIX& GetWorld() { return MatWorld; }

	void MatrixIdentity(const XMVECTOR& Pos, const XMFLOAT3& Rot);

	void Update(const XMVECTOR& vel,const XMFLOAT3& rot,Camera* NormalCam);

	void ShakeCamera(const XMVECTOR& Pos);

private:
	Camera* camera = nullptr;

	//std::unique_ptr<Object3d> debug;
	//ObjModel* debugModel;

	XMMATRIX World;

	// ���[���h�ϊ��s��
	XMMATRIX MatWorld;

	XMMATRIX MatRot, MatTrans;

	XMVECTOR Position;
	XMFLOAT3 Rotation;

	// �r���[�s��
	XMMATRIX MatView = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX MatProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��(�r���[�v���W�F�N�V����)
	XMMATRIX MatViewProjection = DirectX::XMMatrixIdentity();
	// ������x�N�g��
	XMFLOAT3 Up = { 0, 1, 0 };
	//�����_�@�@
	XMFLOAT3 Eye = { 0.0f,0.0f,-5.0f };

	XMFLOAT3 Target = { 0.0f,0.0f,0.0f };

	// �A�X�y�N�g��
	float AspectRatio = 1.0f;

	XMVECTOR EyePosition;
	// �����_���W
	XMVECTOR TargetPosition;
	// �i���́j�����
	XMVECTOR UpVector;
};