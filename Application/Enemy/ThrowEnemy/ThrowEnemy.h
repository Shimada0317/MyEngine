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

public:
	//�I�u�W�F�N�g
	std::unique_ptr<Object3d>HeadPart;
	std::unique_ptr<Object3d>BodyPart;
	std::unique_ptr<Object3d>ArmsPart;
	std::unique_ptr<Object3d>Shadow;
	std::unique_ptr<Object3d>Center;

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
	XMFLOAT3 BodyScl = { 0.f,0.f,0.f };
	//�r
	XMVECTOR ArmsPos = { 0.f,0.f,0.f };
	XMFLOAT3 ArmsRot = { 0.f,0.f,0.f };
	XMFLOAT3 ArmsScl = { 0.f,0.f,0.f };
	//�e�̃X�e�[�^�X
	XMVECTOR ShadowPos = { 0,0,0 };
	XMFLOAT4 ShadowCol = { 0.0f,0.0f,0.0f,0.1f };

	int Hp = 160;
	int OldHp = 0;
};

