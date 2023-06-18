#pragma once
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include"Input.h"
#include"HelperMath.h"

using namespace DirectX;

class Camera;

class Action
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Action();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Action();
public:
	Action(const Action&) = delete;

	Action& operator=(const Action&) = delete;
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns></returns>
	static Action* GetInstance();
	/// <summary>
	/// 3D�A�N�V�����Q�[���p�A�ړ��p�֐�(3D������2D�A�N�V�����Q�[���z��)
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	void PlayerMove3d(XMVECTOR& position);
	/// <summary>
	/// 3D�A�N�V�����Q�[���p�A�W�����v�p�֐�
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	/// <param name="JumpFlag">�W�����v�����Ă��邩�ۂ�</param>
	void PlayerJump(XMFLOAT3& position, bool& JumpFlag);
	/// <summary>
	/// 2D�A�N�V�����Q�[���p�A�ړ��p�֐�
	/// </summary>
	/// <param name="position"></param>
	/// <param name="Speed"></param>
	void PlayerMove2d(XMFLOAT2& position, float Speed);
	/// <summary>
	///  ��������
	/// </summary>
	/// <param name="minValue">�ŏ��͈̔͒l</param>
	/// <param name="maxValue">�ő�͈̔͒l</param>
	float GetRangRand(float minValue, float maxValue);

	/// <summary>
	/// �C�[�Y�A�E�g
	/// </summary>
	/// /// <param name="MoveValue">�ړ�����l</param>
	/// <param name="PurposeValue">�ړI�n�̒l</param>
	void EaseOut(float& MoveValue, float PurposeValue);

	void EasingAddTimer(float& timer_,float addtime_);

	float EasingOut(float time, float addvalue);

	XMFLOAT3 EasingOutXMFLOAT3(float time, const XMFLOAT3& addvalue);

	/// <summary>
	/// �����ɂ���
	/// </summary>
	/// <param name="color">���F</param>
	/// <param name="transparent">���l</param>
	void Flash(float& color,float transparent);

	/// <summary>
	/// �����グ
	/// </summary>
	/// <param name="gravity">�d��</param>
	/// <param name="time">����</param>
	/// <param name="upper">�����グ��l</param>
	/// <param name="position">���ݒn</param>
	void ThrowUp(float gravity, float time, float upper, float& position);
	/// <summary>
	/// ��ʂ̗h��
	/// </summary>
	/// <param name="shakevalue"></param>
	/// <param name="shakingtime"></param>
	/// <param name="eyerot_"></param>
	/// <param name="shakingstartflag_"></param>
	void ScreenShake(float& shakevalue, float shakingtime,XMFLOAT3& eyerot_,bool& shakingstartflag_);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="eyerot_"></param>
	void MoveShakingHead(XMFLOAT3& eyerot_);

	/// <summary>
	/// �x�N�^�[���m�̔�r
	/// </summary>
	/// <param name="FirstValue">1�ڂ̒l</param>
	/// <param name="SecondValue">2�ڂ̒l</param>
	/// <returns></returns>
	bool CompletelyTogetherXMVECTOR(const XMVECTOR& FirstValue, const XMVECTOR& SecondValue);

	void DebugMove(XMFLOAT3& Value);

	void LoopTimer(float& timer,float addvalue,float duration_);

	void ColorDown(XMFLOAT4& color,float downval);

	void ColorUp(XMFLOAT4& color,float upval);

	XMFLOAT3 RandMax(const XMFLOAT3& value,float randvel);

	XMFLOAT3 RandMinAndMax(const XMFLOAT3& minvalue,const XMFLOAT3& maxvalue);

	XMFLOAT2 WorldToScreen(const XMMATRIX& centermat, const XMVECTOR& position,Camera* camera);
private:
	float shakelimittime_ = 0.f;
	float shakingscreenvalue_ = 0.f;
	bool shakingscreenflag_ = false;

	bool shake_ = true;

	bool easing_invert_ = false;
};

