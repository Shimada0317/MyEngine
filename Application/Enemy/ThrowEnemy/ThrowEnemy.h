#pragma once
#include"Camera.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"ParticleManager.h"
#include"Sprite.h"
#include<DirectXMath.h>
#include<list>

class ThrowEnemy
{
private:
	enum State {
		APPEARANCE =0,
		WAIT,
		ATTACK,
		DEATH,
	};

private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:

	//�f�X�g���N�^
	~ThrowEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="allrot">���ʂ̊p�x</param>
	/// <param name="allpos">���ʂ̍��W</param>
	/// <param name="camera">�J����</param>
	/// <param name="trackpos">�Ǐ]��̍��W</param>
	void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos);

	void StatusSet();

	void AllUpdate();

	/// <summary>
	/// ���{�b�g�̍X�V����
	/// </summary>
	/// <param name="bull">�v���C���[�̒e</param>
	/// <param name="playerHp">�v���C���[��HP</param>
	void Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);
	/// <summary>
	/// �o�ꏈ��
	/// </summary>
	void AppearanceProcess();
	/// <summary>
	/// 
	/// </summary>
	void AttackProcess();

	/// <summary>
	/// 2D��3D���W
	/// </summary>
	/// <param name="Set3dPosition">�\��������3D���W�̏ꏊ</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& set3Dposition);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);

	void ThrowAttack();

	/// <summary>
	/// �|����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return DeadFlag; }
public:
	//�I�u�W�F�N�g
	std::unique_ptr<Object3d>HeadPart;
	std::unique_ptr<Object3d>BodyPart;
	std::unique_ptr<Object3d>ArmsPart;
	std::unique_ptr<Object3d>Shadow;
	std::unique_ptr<Object3d>Center;
	std::unique_ptr<Object3d>ThrowBox;
	std::unique_ptr<Object3d>enemy_;
	std::unique_ptr<Object3d>propeller_;

	//�X�v���C�g
	std::unique_ptr<Sprite> RockOn;
	//�J����
	Camera* HaveCamera = nullptr;

	//�p�[�c���Ƃ̃X�e�[�^�X
	//�G�̒��S����
	XMVECTOR CenterWorldPos = { 0.f,0.f,0.f };
	XMVECTOR center_pos_ = { 0.f,0.f,0.f };
	XMFLOAT3 CenterRot = { 0.f,0.f,0.f };
	XMMATRIX CenterMat;
	//
	XMVECTOR body_pos_ = { 0.f,0.f,0.f };
	XMFLOAT3 body_rot_ = { 0.f,0.f,0.f };
	XMFLOAT3 body_scl_ = { 1.f,1.f,1.f };
	//
	XMVECTOR propeller_pos_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_rot_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_scl_{ 1.f,1.f,1.f };
	//�e�̃X�e�[�^�X
	XMVECTOR ShadowPos = { 0.f,0.f,0.f };
	XMFLOAT4 ShadowCol = { 0.f,0.f,0.f,0.1f };
	//���e�n�_
	XMVECTOR LandingPoint = { 0.f,0.f,0.f };
	//�������̍��W
	XMVECTOR ThrowBoxPos = { 0.f,0.f,0.f };
	XMFLOAT3 ThrowBoxRot = { 0.f,0.f,0.f };
	XMFLOAT3 ThrowBoxScl = { 0.f,0.f,0.f };
	//2D���W����������v�Z�Ŏg���ϐ�
	XMVECTOR offset;
	XMMATRIX MatViewPort;
	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 RockOnPos = { 0.0f,0.0f };
	XMFLOAT2 RockOnAnchorPoint = { 0.5f,0.5f };
	XMFLOAT4 RockOnCol = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 RockOnHeadPos = { 0.0f,0.0f };
	XMFLOAT2 RockOnBoxPoss = { 0.f,0.f };

	float bullet_speed_ = 0.f;

	int Hp = 160;
	int OldHp = 0;
	float Length = 0.f;
	//�G�ƃv���C���[�̋���
	float OriginDistance;
	float OriginHeadDistance;
	float OriginBoxDistance;
	float Distance = 60.0f;
	float HeadDistance = 30.0f;
	float BoxDistance = 0.f;
	//Hp��0�ȏォ
	bool DeadFlag = false;
	//�ό`�p�̃t���O
	bool DefomationFlag = false;
	float DefomationCount = 0.0f;

	float floating_pos_ = 0.f;

	int state_ = APPEARANCE;
};

