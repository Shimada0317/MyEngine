#include "DebugCamera.h"
#include <cassert>

using namespace DirectX;

DebugCamera::DebugCamera(int window_width, int window_height)
	: Camera(window_width, window_height)
{
	// ��ʃT�C�Y�ɑ΂��鑊�ΓI�ȃX�P�[���ɒ���
	scaleX = 1.0f / (float)window_width;
	scaleY = 1.0f / (float)window_height;
}

void DebugCamera::Updata()
{
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	//angleX = angX;
	//angleY = angY;



	//if (dirty || viewDirty) {
	//	// �ǉ���]���̉�]�s��𐶐�
	//	XMMATRIX matRotNew = XMMatrixIdentity();
	//	matRotNew *= XMMatrixRotationX(XMConvertToRadians(angleX));
	//	matRotNew *= XMMatrixRotationY(XMConvertToRadians(- angleY));
	//	// �ݐς̉�]�s�������
	//	// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
	//	// �N�H�[�^�j�I�����g�p��������]�܂���
	//	matRot = matRotNew * matRot;

	//	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	//	XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
	//	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };


	//	// �x�N�g������]
	//	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	//	vUp = XMVector3Transform(vUp, matRot);

	//	// �����_���炸�炵���ʒu�Ɏ��_���W������
	//	XMFLOAT3 target = GetEye();

	//	//const XMFLOAT3& eye = GetEye();
	//	SetEye({target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] });
	//	SetUp({ vUp.m128_f32[0], vUp.m128_f32[1],vUp.m128_f32[2] });
	//}

	Camera::Updata();
}
