#pragma once
#include"Camera.h"
#include"DirectXCommon.h"
#include"Object3d.h"
#include"ObjParticle.h"
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

	void WaitProcess();
	/// <summary>
	/// 
	/// </summary>
	void AttackProcess(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	void DamageProcess(const XMFLOAT2& player2Dpos,bool& playerbulletshot);

	void DeathProcess();

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

	void BulletCollision(const XMFLOAT2& player2Dpos, bool& playerbulletshot);

	void ThrowAttack(int& playerhp);

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void ParticleEfect();

	/// <summary>
	/// �|����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return dead_flag_; }
public:
	//�I�u�W�F�N�g
	unique_ptr<Object3d>center_;
	unique_ptr<Object3d>bullet_;
	unique_ptr<Object3d>enemy_;
	unique_ptr<Object3d>propeller_;
	list<unique_ptr<ObjParticle>>obj_particle_;
	//�X�v���C�g
	unique_ptr<Sprite> rockon_;
	unique_ptr<Sprite> rockon_bullet_;
	//�J����
	Camera* bringupcamera_;
	//�_���[�W��H������Ƃ��̃G�t�F�N�g
	ParticleManager* partgreen_ = nullptr;
	ParticleManager* partred_ = nullptr;
	//�p�[�c���Ƃ̃X�e�[�^�X
	//�G�̒��S����
	XMVECTOR center_worldpos_ = { 0.f,0.f,0.f };
	XMVECTOR center_pos_ = { 0.f,0.f,0.f };
	XMFLOAT3 center_rot_ = { 0.f,0.f,0.f };
	XMMATRIX center_mat_;
	//�{�̂̃X�e�[�^�X
	XMVECTOR body_pos_ = {};
	XMFLOAT3 body_rot_ = {};
	XMFLOAT3 body_scl_ = { 1.f,1.f,1.f };
	//�H�̃X�e�[�^�X
	XMVECTOR propeller_pos_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_rot_{ 0.f,0.f,0.f };
	XMFLOAT3 propeller_scl_{ 1.f,1.f,1.f };
	//�e�̃X�e�[�^�X
	XMVECTOR old_pos_{};
	XMVECTOR bullet_pos_{};
	XMFLOAT3 bullet_rot_{};
	XMFLOAT3 bullet_scl_{};
	//���e�n�_
	XMVECTOR landing_point_ = {};
	//2D���W����������v�Z�Ŏg���ϐ�
	XMVECTOR offset_;
	XMMATRIX matviewport_;
	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 rockon_pos_ = { 0.0f,0.0f };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockon_bulletpos_ = { 0.0f,0.0f };
	//�e��
	float bullet_speed_ = 0.05f;
	int hp_ = 50;
	int oldhp_ = hp_;
	float length_ = 0.f;
	//�G�ƃv���C���[�̋���
	float origin_distance_;
	float originhead_distance_;
	float originbox_distance_;
	float distance_ = 60.0f;
	float bullet_distance_ = 0.f;
	bool bullet_active_ = true;
	//Hp��0�ȏォ
	bool dead_flag_ = false;
	bool particle_flag_ = true;
	float floating_pos_ = 0.f;
	float fall_time_ = 0.f;
	int state_ = APPEARANCE;
};

