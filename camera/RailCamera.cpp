#include "RailCamera.h"

using namespace DirectX;


RailCamera::~RailCamera()
{
}

void RailCamera::Initialize(const XMVECTOR& Pos, const XMFLOAT3& Rot)
{
	//Target = { 0.0f,0.0f,-1.0f };
	Eye = { 0,1,-5 };

	//debugModel = ObjModel::CreateFromOBJ("Gear");
	//debug = Object3d::Create(debugModel);


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



void RailCamera::Updata(const XMVECTOR& vel, const XMFLOAT3& rot, Camera* NormalCam)
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
	//debug->SetPosition(Position);
	//debug->Updata();
}

void RailCamera::Draw()
{
	//debug->Draw();
}

void RailCamera::UpdataViewMatrix()
{
	// ���_���W
	EyePosition = XMLoadFloat3(&Eye);
	// �����_���W
	TargetPosition = XMLoadFloat3(&Target);
	// �i���́j�����
	UpVector = XMLoadFloat3(&Up);

	// �J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract(TargetPosition, EyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(UpVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(UpVector));
	// �x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// �J������X���i�E�����j
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(UpVector, cameraAxisZ);
	// �x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	// �J������Y���i������j
	XMVECTOR cameraAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	// �����܂łŒ�������3�����̃x�N�g��������
	//�i���[���h���W�n�ł̃J�����̉E�����A������A�O�����j	

	// �J������]�s��
	XMMATRIX matCameraRot;
	// �J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	MatView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(EyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	MatView.r[3] = translation;

}

void RailCamera::UpdataProjectionMatrix()
{
	// �������e�ɂ��ˉe�s��̐���
	MatProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		AspectRatio,
		0.1f, 1000.0f
	);
}


