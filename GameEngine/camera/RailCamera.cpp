#include "RailCamera.h"

using namespace DirectX;


RailCamera::~RailCamera()
{
}

void RailCamera::MatrixIdentity(const XMVECTOR& Pos, const XMFLOAT3& Rot)
{
	Eye = { 0,1,-5 };

	Position = Pos;
	Rotation = Rot;
	MatRot = XMMatrixIdentity();
	MatRot *= XMMatrixRotationX(XMConvertToRadians(Rotation.x));
	MatRot *= XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	MatRot *= XMMatrixRotationZ(XMConvertToRadians(Rotation.z));
	MatTrans = XMMatrixTranslation(Position.m128_f32[0], Position.m128_f32[1], Position.m128_f32[2]);
	MatWorld = XMMatrixIdentity();
	MatWorld *= MatRot;
	MatWorld *= MatTrans;

	//�r���[�v���W�F�N�V�����̏�����
	MatViewProjection = XMMatrixIdentity();
}



void RailCamera::Update(const XMVECTOR& vel, const XMFLOAT3& rot, Camera* NormalCam)
{
	Position += vel;

	Eye = { 0,1.5,-5 };
	Rotation = { 0.0f,0.0f,0.0f };
	Rotation.x = rot.x;
	Rotation.y = rot.y;
	Rotation.z = rot.z;

	MatRot = XMMatrixIdentity();
	MatRot *= XMMatrixRotationX(XMConvertToRadians(Rotation.x));
	MatRot *= XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	MatRot *= XMMatrixRotationZ(XMConvertToRadians(Rotation.z));
	MatTrans = XMMatrixIdentity();
	MatTrans = XMMatrixTranslation(Position.m128_f32[0], Position.m128_f32[1], Position.m128_f32[2]);
	MatWorld = XMMatrixIdentity();
	MatWorld *= MatRot;
	MatWorld *= MatTrans;

	XMVECTOR eye2 = { Eye.x,Eye.y,Eye.z };
	eye2 = XMVector3Transform(eye2, MatWorld);
	//���[���h�O���x�N�g��
	XMVECTOR forward = { 0, 0, 1 };
	//���[���J�����̉�]�𔽉f
	forward = XMVector3TransformNormal(forward, MatWorld);

	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	Target.x = eye2.m128_f32[0] + forward.m128_f32[0];
	Target.y = eye2.m128_f32[1] + forward.m128_f32[1];
	Target.z = eye2.m128_f32[2] + forward.m128_f32[2];

	//���[���h����x�N�g��
	XMVECTOR Up = { 0,1,0 };
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	Up = XMVector3TransformNormal(Up, MatWorld);
	XMFLOAT3 upp = { Up.m128_f32[0],Up.m128_f32[1],Up.m128_f32[2] };
	Eye = { eye2.m128_f32[0],eye2.m128_f32[1],eye2.m128_f32[2]};


	//�r���[�v���W�F�N�V�������X�V
	NormalCam->SetTarget(Target);
	NormalCam->SetUp(upp);
	NormalCam->SetEye(Eye);
	//�v���C���[�Ƀ��[���h���W�𑗂�
	NormalCam->SetWorld(MatWorld);

}

void RailCamera::ShakeCamera(const XMVECTOR& Pos)
{
	Position = Pos;

	Position.m128_f32[0] += 0.1f;
	Position.m128_f32[1] += 0.1f;
}
