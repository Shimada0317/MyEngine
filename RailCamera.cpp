#include "RailCamera.h"

using namespace DirectX;


RailCamera::~RailCamera()
{
}

void RailCamera::Initialize(const XMVECTOR& Pos, const XMFLOAT3& Rot)
{
	target = { 0.0f,0.0f,-1.0f };
	eye = { 0,1,0 };

	debugModel = ObjModel::CreateFromOBJ("Gear");
	debug = Object3d::Create(debugModel);


	position = Pos;
	rotation = Rot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matTrans = XMMatrixTranslation(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	matWorld = XMMatrixIdentity();
	matWorld *= matRot;
	matWorld *= matTrans;

	//�r���[�v���W�F�N�V�����̏�����
	matViewProjection = XMMatrixIdentity();
}



void RailCamera::Update(const XMVECTOR& vel , const XMFLOAT3& rot )
{
	position += vel;
	rotation.x += rot.x;
	rotation.y += rot.y;
	rotation.z += rot.z;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matTrans = XMMatrixTranslation(position.m128_f32[0], position.m128_f32[1], position.m128_f32[2]);
	matWorld = XMMatrixIdentity();
	matWorld *= matRot;
	matWorld *= matTrans;

	UpdateViewMatrix();

	matViewProjection = matView * matProjection;

	eyePosition = XMVector3TransformNormal(eyePosition, matWorld);
	//���[���h�O���x�N�g��
	XMVECTOR forward = { 0, 0, 1 };
	//���[���J�����̉�]�𔽉f
	forward = XMVector3TransformNormal(forward, matWorld);
		//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	forward = eyePosition + forward;
	//���[���h����x�N�g��
	XMVECTOR up = { 0,1,0 };
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	up = XMVector3TransformNormal(up, matRot);
	//�r���[�v���W�F�N�V�������X�V
	matViewProjection *= matWorld;
	debug->SetPosition(position);
	debug->Update();
}

void RailCamera::Draw()
{
	debug->Draw();
}

void RailCamera::UpdateViewMatrix()
{
	// ���_���W
	eyePosition = XMLoadFloat3(&eye);
	// �����_���W
	targetPosition = XMLoadFloat3(&target);
	// �i���́j�����
	upVector = XMLoadFloat3(&up);

	// �J����Z���i���������j
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	// �x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	// �J������X���i�E�����j
	XMVECTOR cameraAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
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
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	// �J�����̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);	// X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);	// Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);	// Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

}

void RailCamera::UpdateProjectionMatrix()
{
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, 1000.0f
	);
}


