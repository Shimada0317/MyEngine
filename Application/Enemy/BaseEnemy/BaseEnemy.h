#pragma once
#include"Action.h"
#include"Audio.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include"ParticleManager.h"
#include"Collision.h"

using namespace DirectX;

class Player;

class BaseEnemy
{

protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://���ʂ̉��z�֐�
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="came">�J����</param>
	virtual void CreateRobot(Camera* camera) = 0;
	
	
	virtual void Activity(Player* player) = 0;
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	virtual void Draw(DirectXCommon* dxCommon) = 0;

	/// <summary>
	/// ���{�b�g�̍X�V����
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Update(Player* player);

	bool IsDead()const { return dead_flag_; }
protected://�e�N���X���Ŏg���鏈��
	

	void CommonLoad();

	void CommonStatusSet();

	void CommonUpdate();

	void GetPlayerState(Player* player);

	void ObjParticleDelete();

	void CommonCollision();

	void TrackCalculation();

	void CommonParticleDraw(DirectXCommon* dxCommon);

	void CommonObjDraw();

	void ParticleEffect();

	void Damage();
protected://���ʂ̕ϐ�
	//�e
	std::unique_ptr<Object3d> shadow_;
	//���S
	std::unique_ptr<Object3d> center_;
		//�X�v���C�g
	unique_ptr<Sprite> rockon_body_;
	unique_ptr<Sprite> rockon_other_;
	//Obj�p�[�e�B�N��
	std::list<std::unique_ptr<ObjParticle>>obj_particle_;
	//�_���[�W��H������Ƃ��̃G�t�F�N�g
	ParticleManager* partgreen_ = nullptr;
	ParticleManager* partred_ = nullptr;
	//�J����
	Camera* bringupcamera_;

	//�ǔ��̌v�Z
	XMFLOAT3 value_;
	//�̂̒��S�̍��W
	XMVECTOR base_pos_{};
	XMFLOAT3 all_rot_;
	XMFLOAT3 base_rot_{};
	//�e�̃X�e�[�^�X
	XMVECTOR shadow_pos_ {};
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };
	//�G�̒��S����
	XMVECTOR center_worldpos_ {};
	XMFLOAT3 center_rot_ {};
	XMMATRIX center_mat_;
	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 rockon_pos_ {};
	XMFLOAT2 rockonother_pos_ {};
	XMFLOAT2 anchorpoint_ {0.5f,0.5f };
	XMFLOAT4 rockon_color_ {1.f,1.f,1.f,1.f};
	//�G�������Ă���X�e�[�^�X
	int hp_ = 50;
	int old_hp_ = 0;
	float length_ = 0.f;
	float mul_value_ = 0.f;
	int damage_value_ = 0;
	//�ړ����x
	float speed_ = 0.f;
	XMVECTOR TrackSpeed{};
	XMVECTOR track_point_ {};
	XMVECTOR oldtrack_point_ {};
	//�����Ă����v���C���[�̏��
	Player* player_;
	XMFLOAT2 player_pos_{};
	bool player_shot_ = false;
	int player_hp_ = 5;
	//�G�ƃv���C���[�̋���
	float origin_distance_;
	float originother_distance_;
	float originbox_distance_;
	float distance_ = 60.0f;
	//Hp��0�ȏォ
	bool dead_flag_ = false;

	float start_scl_ = 4.f;
	float end_scl_ = 0.f;
	float time_ = 150.f;
	int roop_count_ = 50;
	int file = 200;
};

