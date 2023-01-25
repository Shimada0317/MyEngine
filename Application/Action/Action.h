#pragma once
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include"Input.h"

class Action
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
	/// ���{�b�g�̍X�V����
	/// </summary>
	/// <param name="minValue">�ŏ��͈̔͒l</param>
	/// <param name="maxValue">�ő�͈̔͒l</param>
	float GetRangRand(float minValue, float maxValue);
};

