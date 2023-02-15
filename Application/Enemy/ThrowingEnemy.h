#pragma once
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include<DirectXMath.h>
#include<memory>
#include<list>


class ThrowingEnemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="allRot">��]</param>
	/// <param name="AllPos">���W</param>
	/// <param name="came">�J����</param>
	/// <param name="Step">�ړ����ɉ��ړ����邩</param>
	void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera);

	/// <summary>
	/// �X�e�[�^�X���Z�b�g
	/// </summary>
	void StatusSet();

	/// <summary>
	/// Obj����Update�͂��̒���
	/// </summary>
	/// <param name="bull">�v���C���[�̒e</param>
	void AllUpdate();

	/// <summary>
	/// ���{�b�g�̍X�V����
	/// </summary>
	/// <param name="bull">�v���C���[�̒e</param>
	/// <param name="playerHp">�v���C���[��HP</param>
	void Update(const XMFLOAT2& Player2DPos, int& PlayerHp, bool& PlayerBulletShot);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);
private:
	//Obj
	//��
	std::unique_ptr<Object3d> HeadPart;
	//���r
	std::unique_ptr<Object3d> ArmsPart;
	//��
	std::unique_ptr<Object3d> BodyPart;
	//�e
	std::unique_ptr<Object3d> Shadow;
	//���S
	std::unique_ptr<Object3d> Center;
	//Obj�p�[�e�B�N��
	std::list<std::unique_ptr<ObjParticle>>Obj_Particle;

	//�X�v���C�g
	//��
	std::unique_ptr<Sprite> RockOn;
	//��
	std::unique_ptr<Sprite> RockOnHead;

	//�J����
	Camera* BringUpCamera;

	//�G�̒��S�����̃X�e�[�^�X
	XMVECTOR CenterWorldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 CenterRot = { 0.0f,0.0f,0.0f };
	XMMATRIX CenterMat;

	//�G�������Ă���X�e�[�^�X
	int Hp = 50;
	int OldHp = 0;
	bool RobotArive = false;
	XMVECTOR TrackPoint = { 0,0,0 };

	//�p�[�c���Ƃ̃X�P�[��
	XMFLOAT3 HeadPartScl = { 0.3f,0.3f,0.3f };
	XMFLOAT3 BodyPartScl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 ArmsPartScl = { 0.2f,0.2f,0.2f };

	//�p�[�c���Ƃ̃|�W�V����
	XMVECTOR HeadPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR BodyPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR ArmsPartPos = { 0.0f,0.0f,0.0f };

	//�p�[�c���Ƃ̐F
	XMFLOAT4 ArmsPartColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 HeadPartColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 BodyPartColor = { 1.0f,1.0f,1.0f,1.0f };

	//�p�[�c���Ƃ̉�]
	XMFLOAT3 ArmsPartRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 HeadPartRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 BodyPartRot = { 0.0f,0.0f,0.0f };

	//�p�[�c���Ƃɓn���X�e�[�^�X
	XMVECTOR AllPos = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 AllRot;

	//�e�̃X�e�[�^�X
	XMVECTOR ShadowPos = { 0,0,0 };
	XMFLOAT4 ShadowCol = { 0.0f,0.0f,0.0f,0.1f };

	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 RockOnPos = { 0.0f,0.0f };
	XMFLOAT2 RockOnAnchorPoint = { 0.5f,0.5f };
	XMFLOAT4 RockOnCol = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 RockOnHeadPos = { 0.0f,0.0f };

	//�G�ƃv���C���[�̋���
	float OriginDistance;
	float OriginHeadDistance;

	float Distance = 60.0f;
	float HeadDistance = 30.0f;

	//�G�̃��[�V�����p
	float PursePositiveRot = 180;
	float PurseNegativeeRot = 0;
};

