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

	//�X�v���C�g
	std::unique_ptr<Sprite> RockOn;
	//�J����
	Camera* HaveCamera = nullptr;

	//�p�[�c���Ƃ̃X�e�[�^�X
	//�G�̒��S����
	XMVECTOR CenterWorldPos = { 0.f,0.f,0.f };
	XMVECTOR AllPos = { 0.f,0.f,0.f };
	XMFLOAT3 CenterRot = { 0.f,0.f,0.f };
	XMMATRIX CenterMat;
	//��
	XMVECTOR HeadPos = { 0.f,0.f,0.f };
	XMFLOAT3 HeadRot = { 0.f,0.f,0.f };
	XMFLOAT3 HeadScl = { 0.f,0.f,0.f };
	//��
	XMVECTOR BodyPos = { 0.f,0.f,0.f };
	XMFLOAT3 BodyRot = { 0.f,0.f,0.f };
	XMFLOAT3 BodyScl = { 1.f,1.f,1.f };
	//�r
	XMVECTOR ArmsPos = { 0.f,0.f,0.f };
	XMFLOAT3 ArmsRot = { 0.f,0.f,0.f };
	XMFLOAT3 ArmsScl = { 0.f,0.f,0.f };
	//�e�̃X�e�[�^�X
	XMVECTOR ShadowPos = { 0.f,0.f,0.f };
	XMFLOAT4 ShadowCol = { 0.f,0.f,0.f,0.1f };
	//���e�n�_
	XMVECTOR LandingPoint = { 0.f,0.f,0.f };
	//�������̍��W
	XMVECTOR ThrowBoxPos = { 0.f,0.f,0.f };
	XMFLOAT3 ThrowBoxRot = { 0.f,0.f,0.f };
	XMFLOAT3 ThrowBoxScl = { 0.f,0.f,0.f };


	int Hp = 160;
	int OldHp = 0;
	float Length = 0.f;
	float MoveSpeed = 0.1f;
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

};

