#pragma once
#include"Audio.h"
#include"Body.h"
#include"BothArms.h"
#include"Head.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Texture.h"
#include"TextureModel.h"
#include"Sprite.h"
#include"Camera.h"
#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class Robot
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	//�f�X�g���N�^
	~Robot();
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="allRot">��]</param>
	/// <param name="AllPos">���W</param>
	/// <param name="came">�J����</param>
	/// <param name="Step">�ړ����ɉ��ړ����邩</param>
	void Initialize(const XMFLOAT3& allRot, const XMVECTOR& AllPos, Camera* came, const bool& movement= false);
	/// <summary>
	/// Upfdate�͂��̒���
	/// </summary>
	/// <param name="bull">�v���C���[�̒e</param>
	void AllUpdata(Bullet* bull);
	/// <summary>
	/// ���{�b�g�̍X�V����
	/// </summary>
	/// <param name="bull">�v���C���[�̒e</param>
	/// <param name="playerHp">�v���C���[��HP</param>
	void Update(Bullet* bull,int& playerHp);
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//Img�`��
	void ImgDraw();
	//�p�[�e�B�N���`��
	void ParticleDraw(DirectXCommon* dxCommon);
	//�v���C���[�ǔ����[�h
	void TrackPlayerMode();
	//��U��
	void Motion();
	//�U�����[�h
	void AttackMode(int& playerHp);
	//2D��3D���W
	void WorldtoScreen();
	//�r���[�ϊ�
	void ChangeViewPort(XMMATRIX& matViewPort);

public://Getter Setter
	void SetPosition(XMVECTOR allPos) { this->All_Pos = allPos; }

	void SetRotation(XMFLOAT3 allRot) { this->All_Rot = allRot; }

	void SetTrackPoint(XMVECTOR TrackPoint) { this->TrackPoint = TrackPoint; }

	const XMVECTOR& GetPosition() { return Center_WorldPos; }

	const XMFLOAT2& Get2DPosition() { return RockOn_Pos; }

	bool IsDead() const { return isDead_; }

private:
	//�̂��Ƃ̃p�[�c
	std::unique_ptr<Head> head;
	std::unique_ptr<BothArms>arms;
	std::unique_ptr<Body>body;

	std::unique_ptr<Object3d> HeadPart;
	std::unique_ptr<Object3d> ArmsPart;
	std::unique_ptr<Object3d> BodyPart;

	//Obj
	std::unique_ptr<Object3d> Shadow;
	std::unique_ptr<Object3d> Center;
	//Obj���f��
	ObjModel* ShadowModel = nullptr;
	ObjModel* HeadPartModel;
	ObjModel* ArmsPartModel;
	ObjModel* BodyPartModel;
	//Obj�p�[�e�B�N��
	std::list<std::unique_ptr<ObjParticle>>Obj_Particle;
	//�X�v���C�g
	std::unique_ptr<Sprite> RockOn;
	//���̑�
	Audio* Clush_SE;
	Camera* camera;
	//�G�̒��S�����̃X�e�[�^�X
	XMVECTOR Center_WorldPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Center_Rot = { 0.0f,0.0f,0.0f };
	//�G�������Ă���X�e�[�^�X
	int Hp = 50;
	int OldHp = 0;
	bool Robotarive = false;
	XMVECTOR TrackPoint = { 0,0,0 };
	//�p�[�c���Ƃ̃X�P�[��
	XMFLOAT3 HeadPartScl = { 0.3f,0.3f,0.3f };
	XMFLOAT3 BodyPartScl = { 1.0f,1.0f,1.0f };
	XMFLOAT3 ArmsPartScl = { 0.2f,0.2f,0.2f };
	XMVECTOR HeadPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR BodyPartPos = { 0.0f,0.0f,0.0f };
	XMVECTOR ArmsPartPos = { 0.0f,0.0f,0.0f };
		//�p�[�c���Ƃɓn���X�e�[�^�X
		XMVECTOR All_Pos = { 0.0f,0.0f,-10.0f };
		XMFLOAT3 All_Rot;
		//���ʔj��p
		bool RemainPart[3];
	//�e�̃X�e�[�^�X
	XMVECTOR Shadow_Pos = { 0,0,0 };
	XMFLOAT4 Shadow_Col = { 0.0f,0.0f,0.0f,0.1f };
	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 RockOn_Pos = { 0.0f,0.0f };
	XMFLOAT2 RockOn_Anc = { 0.5f,0.5f };
	XMFLOAT4 RockOn_Col = { 1.0f,1.0f,1.0f,1.0f };
	//2D���W����������v�Z�Ŏg���ϐ�
	XMVECTOR offset;
	XMMATRIX Center_Mat;
	XMMATRIX MatViewPort;
	//�U�����[�h�Ŏg�p�����ϐ�
	float AttackTime = 0.0f;
	float AttackChanse = 0;
	float Rand = 0;
	bool AttackFase = false;
	float AttackPreparationTime = 0;
	//�ړ����x
	float MoveSpeed = 0.005f;
	//�v���C���[�ƓG�̋���
	float Length = 0;
	//�ҋ@���̃��[�V�����ϐ�
	float MotionTime = 0.0f;
	bool MotionChange = true;
	//�T�C�h�X�e�b�v
	float MovementChangeTime = 0.0f;
	bool Reversal_F = false;
	float SideStepSpeed = 0.001f;
	bool Movement_F = false;
	//Hp��0�ȏォ
	bool isDead_ = false;
};

