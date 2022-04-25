#include "Camera.h"

using namespace DirectX;

Camera::Camera(int window_width, int window_height)
{
	aspect = (float)window_width / window_height;
	//�r���[�s��
	UpdatematView();
	//�ˉe�s��
	UpdatematProjection();
	//�r���[�v���W�F�N�V����
	matViewProjection = matView * matProjection;
}

void Camera::Iniitialize()
{
}

void Camera::Update()
{
	if (viewflag || projectionflag) {
		//�r���[�s��̍Čv�Z������Ȃ�
		if (viewflag) {
			//�Čv�Z(�X�V)
			UpdatematView();
			viewflag = false;
		}
		//�ˉe�s��̍Čv�Z������Ȃ�
		if (projectionflag) {
			//�Čv�Z(�X�V)
			UpdatematProjection();
			projectionflag = false;
		}
		//�r���[�v���W�F�N�V����
		matViewProjection = matView * matProjection;
	}

}

void Camera::UpdatematView()
{
	//���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&eye);
	//�����_���W
	XMVECTOR targetPos = XMLoadFloat3(&target);
	//�����
	XMVECTOR upVecror = XMLoadFloat3(&up);

	//�J����Z��
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPos, eyePosition);
	//0�x�N�g�����ƌ�������܂�Ȃ��̂�
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVecror, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVecror));
	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	//�J������X��
	XMVECTOR cameraAxisX;
	//X���͏������X���̊O��
	cameraAxisX = XMVector3Cross(upVecror, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������Y��
	XMVECTOR cameraAxisY;
	//Y��
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]
	XMMATRIX matCameraRot;
	//�J�������W���烏�[���h���W
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	//�]�u�ɂ��t�s��v�Z
	matView = XMMatrixTranspose(matCameraRot);
	//���_���W��-1���|�����v�Z
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��
	XMVECTOR tx = XMVector3Dot(cameraAxisX, reverseEyePosition);//X
	XMVECTOR ty = XMVector3Dot(cameraAxisY, reverseEyePosition);//Y
	XMVECTOR tz = XMVector3Dot(cameraAxisZ, reverseEyePosition);//Z
	//1�ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tx.m128_f32[0], ty.m128_f32[1], tz.m128_f32[2], 1.0f);
	//�r���[�s��ɕ��s�ړ�
	matView.r[3] = translation;

	//�r���{�[�h�s��
	matBillbord.r[0] = cameraAxisX;
	matBillbord.r[1] = cameraAxisY;
	matBillbord.r[2] = cameraAxisZ;
	matBillbord.r[3] = XMVectorSet(0, 0, 0, 1);

	//�J����X���AY���AZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVecror);
	//Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y�����r���{�[�h�s��
	matBillbordY.r[0] = ybillCameraAxisX;
	matBillbordY.r[1] = ybillCameraAxisY;
	matBillbordY.r[2] = ybillCameraAxisZ;
	matBillbordY.r[3] = XMVectorSet(0, 0, 0, 1);

}

void Camera::UpdatematProjection()
{
	//�������e�ɂ��ˉe�s��
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspect,
		0.1f, 1000.0f
	);
}

void Camera::MoveEyeVectror(const XMFLOAT3& move)
{
	//���_���W���ړ����A���f
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	SetEye(eye_moved);
}

void Camera::MoveEyeVector(const XMVECTOR& move)
{
	//���_���W���ړ����A���f
	XMFLOAT3 eye_moved = GetEye();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
}

void Camera::MoveVector(const XMFLOAT3& move)
{
	//���_�ƒ����_���W���ړ����A���f
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.x;
	eye_moved.y += move.y;
	eye_moved.z += move.z;

	target_moved.x += move.x;
	target_moved.y += move.y;
	target_moved.z += move.z;

	SetEye(eye_moved);
	SetTarget(target_moved);
}

void Camera::MoveVector(const XMVECTOR& move)
{
	//���_�ƒ����_���W���ړ����A���f
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	target_moved.x += move.m128_f32[0];
	target_moved.y += move.m128_f32[1];
	target_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
	SetTarget(target_moved);
}