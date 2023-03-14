#pragma once
#include"Audio.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include"Camera.h"
#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class Enemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//�f�X�g���N�^
	~Enemy();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="allRot">��]</param>
	/// <param name="allPos">���W</param>
	/// <param name="came">�J����</param>
	/// <param name="Step">�ړ����ɉ��ړ����邩</param>
	void Initialize(const XMFLOAT3& allRot, const XMVECTOR& allPos, Camera* camera, const XMVECTOR& trackpoint, const bool& movement= false);

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
	void Update(const XMFLOAT2& player2Dpos, int& playerhp,bool& playerbulletshot);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
	/// �v���C���[�ǔ����[�h
	/// </summary>
	void TrackPlayerMode();

	/// <summary>
	/// �ҋ@���[�V����
	/// </summary>
	void Motion();

	/// <summary>
	/// �U�����[�h
	/// </summary>
	/// <param name="playerHp">�v���C���[��Hp</param>
	void AttackMode(int& playerHp);

	/// <summary>
	/// �U��
	/// </summary>
	/// <param name="playerhp">�v���C���[��Hp</param>
	void Attack(int& playerhp,float& attacktimer);

	/// <summary>
	/// 2D��3D���W
	/// </summary>
	/// <param name="Set3dPosition">�\��������3D���W�̏ꏊ</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& set3Dposition);

	/// <summary>
	/// �r���[�ϊ�
	/// </summary>
	void ChangeViewPort(XMMATRIX& matviewport);

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void ParticleEfect();

	void WaitTrack(bool otherenemyarive);

public://Getter Setter
	/// <summary>
	/// �ǔ���̃Z�b�^�[
	/// </summary>
	/// <param name="TrackPoint">�ǔ���</param>
	void SetTrackPoint(const XMVECTOR& trackpoint) { this->OldTrackPoint = trackpoint; }

	/// <summary>
	/// �ǔ���̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetTrackPos() { return OldTrackPoint; }

	/// <summary>
	/// ���S�̃��[���h���W�̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetPosition() { return CenterWorldPos; }

	/// <summary>
	/// 2D���W�̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	const XMFLOAT2& Get2DPosition() { return RockOnPos; }

	/// <summary>
	/// �|����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return DeadFlag; }

	bool GetArive() { return RobotAriveFlag; }

	bool GetCountZero() { return CountZeroFlag; }

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

	//�_���[�W��H������Ƃ��̃G�t�F�N�g
	ParticleManager* PartGreen = nullptr;
	ParticleManager* PartRed = nullptr;

	//���̑�
	//�T�E���h�G�t�F�N�g
	Audio* ClushSe;
	//�J����
	Camera* BringUpCamera;

	//�G�̒��S�����̃X�e�[�^�X
	XMVECTOR CenterWorldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 CenterRot = { 0.0f,0.0f,0.0f };
	XMMATRIX CenterMat;

	//�G�������Ă���X�e�[�^�X
	int Hp = 50;
	int OldHp = 0;
	bool RobotAriveFlag = false;
	XMVECTOR TrackPoint = { 0,0,0 };
	XMVECTOR OldTrackPoint = { 0,0,0 };
	XMVECTOR FakeTrackPoint = { 0,0,0 };

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

	//2D���W����������v�Z�Ŏg���ϐ�
	XMVECTOR offset;
	XMMATRIX MatViewPort;

	//�U�����[�h�Ŏg�p�����ϐ�
	float AttackTime = 0.0f;
	int AttackChanse = 0;
	float Rand = 0;
	bool AttackFaseFlag = false;

	//�U���̏�������
	float AttackPreparationTime = 0;
	bool AttackShakeDownFlag = false;
	float AttackCharge = 0.0f;

	//�ړ����x
	float MoveSpeed = 0.09f;

	//�v���C���[�ƓG�̋���
	float Length = 3.0f;
	float LengthLimit = 1.5f;

	//�ҋ@���̃��[�V�����ϐ�
	float MotionTime = 0.0f;
	bool MotionChangeFlag = true;

	//�T�C�h�X�e�b�v
	float MovementChangeTime = 0.0f;
	bool ReversalFlag = false;
	float SideStepSpeed = 0.001f;
	bool MovementFlag = false;

	//Hp��0�ȏォ
	bool DeadFlag = false;

	//�G�ƃv���C���[�̋���
	float OriginDistance;
	float OriginHeadDistance;

	float Distance = 60.0f;
	float HeadDistance = 30.0f;

	//�G�̃��[�V�����p
	float PursePositiveRot = 180;
	float PurseNegativeeRot = 0;

	//�ό`�p�̃t���O
	bool DefomationFlag = false;
	float DefomationCount = 0.0f;

	//�U��
	float Vibration=0.0f;
	bool VibrationChangeFlag = false;

	XMVECTOR AttackBeforePos = { 0.0f,0.0f,0.0f };
	float LimitDistance = 0.0f;

	float AtttackTimer = 0.f;
	float TimerLimit = 0.f;
	bool RandomFlag = false;
	float AttackTimeMin = 20;
	float AttackTimeMax = 40;

	bool ParticleEfectFlag = false;
	//�����Ǐ]��ɕʂ̓G�����邩���Ȃ���
	bool WaitFlag = false;

	bool CountZeroFlag = false;
};

