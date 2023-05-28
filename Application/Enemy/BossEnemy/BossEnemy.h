#pragma once
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"

#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class Camera;

class BossEnemy
{
private: 
	enum State {
		kAppearance,
		kMove,
		kStun,
		kDead,
	}state_;
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

	void Appearance();

	void Move(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	void Stun(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot);

	/// <summary>
/// �v���C���[�ǔ����[�h
/// </summary>
	void TrackPlayerMode();

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
	//�R�A
	std::unique_ptr<Sprite> rockoncore_;

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
	int hp_ = 600;
	int oldhp_ = hp_;
	bool robotarive_flag_ = true;
	XMVECTOR track_point_ = { 0,0,0 };
	//�p�[�c���Ƃ̃X�P�[��
	XMFLOAT3 headpart_scl_ = { 0.f,0.f,0.f };
	XMFLOAT3 bodypart_scl_ = { 0.f,0.f,0.f };
	XMFLOAT3 corepart_scl_ = { 0.f,0.f,0.f };
	//�p�[�c���Ƃ̃|�W�V����
	XMVECTOR headpart_pos_ = { 0.0f,-1000.0f,0.0f };
	XMVECTOR bodypart_pos_ = { 0.0f,-1000.0f,0.0f };
	XMVECTOR corepart_pos_ = { 0.f,1000.f,0.f };
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
	XMVECTOR shadow_pos_ = {};
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };

	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 rockon_pos_ = {};
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockonhead_pos_ = {};
	XMFLOAT2 rockoncore_pos_ = {};

	//2D���W����������v�Z�Ŏg���ϐ�
	XMVECTOR offset_;
	XMMATRIX matviewport_;

	float attack_charge_ = 0.0f;
	//�ړ����x
	float movespeed_ = 0.07f;
	//�v���C���[�ƓG�̋���
	float length_ = 13.0f;
	float limit_length_ = 7.0f;
	//Hp��0�ȏォ
	bool dead_flag_ = false;
	//�G�ƃv���C���[�̋���
	float origin_distance_;
	float originhead_distance_;
	float origincore_distance_;
	float distance_ = 60.f;
	float head_distance_ = 30.f;
	float core_distance_=20.f;

	float limit_distance_ = 0.f;
	float atttack_timer_ = 0.f;
	//�����Ǐ]��ɕʂ̓G�����邩���Ȃ���
	bool objparticle_flag_ = false;

	float addrot_ = 20.f;
	float time_ = 0.f;
	bool particleefect_flag_ = true;
};

