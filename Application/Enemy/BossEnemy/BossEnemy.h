#pragma once
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include"Camera.h"
#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class BossEnemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//�f�X�g���N�^
	~BossEnemy();

	void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint);

	void StatusSet();

	void AllUpdate();

	void Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	void Draw(DirectXCommon* dxCommon);

	/// <summary>
/// �v���C���[�ǔ����[�h
/// </summary>
	void TrackPlayerMode();

	/// <summary>
	/// �U�����[�h
	/// </summary>
	/// <param name="playerHp">�v���C���[��Hp</param>
	void AttackMode(int& playerhp);

	/// <summary>
	/// �U��
	/// </summary>
	/// <param name="playerhp">�v���C���[��Hp</param>
	void Attack(int& playerhp, float& attacktimer);

	void Damage();

	void Death();

	/// <summary>
	/// 2D��3D���W
	/// </summary>
	/// <param name="Set3dPosition">�\��������3D���W�̏ꏊ</param>
	XMFLOAT2 WorldtoScreen(const XMVECTOR& set3Dposition);

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void ParticleEfect();

	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void SelfDestructingEfect();

	void WaitTrack(bool otherenemyarive);
public:

	/// <summary>
/// �|����Ă��邩
/// </summary>
/// <returns></returns>
	bool IsDead() const { return dead_flag_; }

private:
	//Obj
//��
	std::unique_ptr<Object3d> headpart_;
	//��
	std::unique_ptr<Object3d> bodypart_;
	//��
	std::unique_ptr<Object3d> corepart_;
	//�e
	std::unique_ptr<Object3d> shadow_;
	//���S
	std::unique_ptr<Object3d> center_;
	//Obj�p�[�e�B�N��
	std::list<std::unique_ptr<ObjParticle>>obj_particle_;

	//�X�v���C�g
	//��
	std::unique_ptr<Sprite> rockon_;
	//��
	std::unique_ptr<Sprite> rockonhead_;

	//�_���[�W��H������Ƃ��̃G�t�F�N�g
	ParticleManager* partgreen_ = nullptr;
	ParticleManager* partred_ = nullptr;

	//�J����
	Camera* bringupcamera_;
	//�G�̒��S�����̃X�e�[�^�X
	XMVECTOR center_worldpos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 center_rot_ = { 0.0f,0.0f,0.0f };
	XMMATRIX center_mat_;

	//�G�������Ă���X�e�[�^�X
	int hp_ = 50;
	int oldhp_ = 0;
	bool robotarive_flag_ = false;
	XMVECTOR track_point_ = { 0,0,0 };
	XMVECTOR oldtrack_point_ = { 0,0,0 };
	XMVECTOR faketrack_point_ = { 0,0,0 };
	//�p�[�c���Ƃ̃X�P�[��
	XMFLOAT3 headpart_scl_ = { 1.2f,1.2f,1.2f };
	XMFLOAT3 bodypart_scl_ = { 2.3f,2.3f,2.3f };
	XMFLOAT3 corepart_scl_ = { 2.f,2.f,2.f };
	//�p�[�c���Ƃ̃|�W�V����
	XMVECTOR headpart_pos_ = { 0.0f,-1000.0f,0.0f };
	XMVECTOR bodypart_pos_ = { 0.0f,-1000.0f,0.0f };
	XMVECTOR corepart_pos_ = { 0.f,-1000.f,0.f };
	//�p�[�c���Ƃ̐F
	XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 bodypart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 corepart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	//�p�[�c���Ƃ̉�]
	XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodypart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 corepart_rot_ = { 0.0f,0.0f,0.0f };
	//�p�[�c���Ƃɓn���X�e�[�^�X
	XMVECTOR all_pos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 all_rot_;

	//�e�̃X�e�[�^�X
	XMVECTOR shadow_pos_ = { 0,0,0 };
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };

	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 rockon_pos_ = { 0.0f,0.0f };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockonhead_pos_ = { 0.0f,0.0f };

	//2D���W����������v�Z�Ŏg���ϐ�
	XMVECTOR offset_;
	XMMATRIX matviewport_;
	//�U�����[�h�Ŏg�p�����ϐ�
	bool attackfase_flag_ = false;
	//�U���̏�������
	bool attackshakedown_flag_ = false;
	float attack_charge_ = 0.0f;
	//�ړ����x
	float movespeed_ = 0.03f;
	//�v���C���[�ƓG�̋���
	float length_ = 13.0f;
	float limit_length_ = 7.0f;
	//Hp��0�ȏォ
	bool dead_flag_ = false;
	//�G�ƃv���C���[�̋���
	float origin_distance_;
	float originhead_distance_;
	float distance_ = 60.0f;
	float head_distance_ = 30.0f;
	//�G�̃��[�V�����p
	float purse_positiverot_ = 180;
	float purse_negativerot_ = 0;
	//�ό`�p�̃t���O
	bool defomation_flag_ = false;
	float defomation_count_ = 0.0f;
	//�U��
	float vibration_ = 0.0f;
	bool vibrationchange_flag_ = false;
	XMVECTOR attack_beforepos_ = { 0.0f,0.0f,0.0f };
	float limit_distance_ = 0.0f;
	float atttack_timer_ = 0.f;
	float timer_limit_ = 0.f;
	bool random_flag_ = false;
	float attacktime_min_ = 20;
	float attacktime_max_ = 40;
	bool particleefect_flag_ = true;
	//�����Ǐ]��ɕʂ̓G�����邩���Ȃ���
	bool wait_flag_ = false;
	bool notlife_flag_ = false;
	bool objparticle_flag_ = false;

};

