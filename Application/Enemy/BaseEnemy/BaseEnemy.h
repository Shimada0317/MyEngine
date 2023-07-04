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
	enum State {
		kDefomation,
		kMove,
		kWait,
		kAttack,
		kDeath,
	}state_;
protected:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//���z�֐�

	virtual void Activity() = 0;
public:
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="came">�J����</param>
	void Initialize(Camera* camera);
	/// <summary>
	/// �X�e�[�^�X���Z�b�g
	/// </summary>
	void StatusSet();
	/// <summary>
	/// Obj����Update�͂��̒���
	/// </summary>
	void AllUpdate();
	/// <summary>
	/// ���{�b�g�̍X�V����
	/// </summary>
	/// <param name="player">�v���C���[</param>
	void Update(Player* player);
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);
protected:
	/// <summary>
	/// �v���C���[�ǔ����[�h
	/// </summary>
	void TrackPlayerMode();
	/// <summary>
/// �_���[�W��H�����
/// </summary>
	void Damage();
	/// <summary>
	/// ����
	/// </summary>
	void Death();

	bool CheckHit(bool& playershot, XMFLOAT2 playerppos, XMFLOAT2 rockonpos, float distance, float mulvalue);

	void ParticleEfect();
	/// <summary>
	/// �����ɂ���
	/// </summary>
	void Tracsparentsize();
	/// <summary>
	/// �U�����󂯂��Ƃ��ɐF��ς���
	/// </summary>
	void HitColor();
	/// <summary>
	/// �U���`���[�W
	/// </summary>
	void AttackCharge();
	/// <summary>
	/// �h�炷����
	/// </summary>
	void ShakeBody();
	/// <summary>
	/// �U��
	/// </summary>
	void Attack();
	/// <summary>
	/// �����ɂ���
	/// </summary>
	void Transparentize();
	/// <summary>
	/// �p�[�c��傫������
	/// </summary>
	void Enlargement();

	void AttackBefore();
	/// <summary>
	/// �W�J���̑傫��
	/// </summary>
	void DeploymentScale();
private:

	bool CheckShot(bool playershot);
public://Getter Setter
/// <summary>
/// �ǔ���̃Z�b�^�[
/// </summary>
/// <param name="TrackPoint">�ǔ���</param>
	void SetTrackPoint(const XMVECTOR& trackpoint) { this->oldtrack_point_ = trackpoint; }
	/// <summary>
	/// �ǔ���̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetTrackPos() { return oldtrack_point_; }

	const XMVECTOR& CheckTrackPoint() { return track_point_; }
	/// <summary>
	/// ���S�̃��[���h���W�̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	const XMVECTOR& GetPosition() { return center_worldpos_; }
	/// <summary>
	/// �|����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return dead_flag_; }

protected:
	//Obj
	//��
	std::unique_ptr<Object3d> headpart_;
	//���r
	std::unique_ptr<Object3d> armspart_;
	//��
	std::unique_ptr<Object3d> bodypart_;
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
	//���̑�
	//�J����
	Camera* bringupcamera_;
	//�G�̒��S�����̃X�e�[�^�X
	XMVECTOR center_worldpos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 center_rot_ = { 0.0f,0.0f,0.0f };
	XMMATRIX center_mat_;
	//�G�������Ă���X�e�[�^�X
	int base_hp_ = 50;
	int old_hp_ = 0;
	XMVECTOR track_point_ = { 0,0,0 };
	XMVECTOR oldtrack_point_ = { 0,0,0 };
	//�p�[�c���Ƃ̃X�P�[��
	XMFLOAT3 headpart_scl_ = { 0.3f,0.3f,0.3f };
	XMFLOAT3 bodypart_scl_ = { 1.0f,1.0f,1.0f };
	XMFLOAT3 armspart_scl_ = { 0.2f,0.2f,0.2f };
	//�p�[�c���Ƃ̃|�W�V����
	XMVECTOR headpart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR bodypart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR armspart_pos_ = { 0.0f,0.0f,0.0f };
	//�p�[�c���Ƃ̐F
	XMFLOAT4 armspart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 bodypart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	//�p�[�c���Ƃ̉�]
	XMFLOAT3 armspart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodypart_rot_ = { 0.0f,0.0f,0.0f };
	//�p�[�c���Ƃɓn���X�e�[�^�X
	XMVECTOR base_pos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 all_rot_;
	//�e�̃X�e�[�^�X
	XMVECTOR shadow_pos_ = { 0,0,0 };
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };
	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 rockon_pos_ = { 0.0f,0.0f };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockonhead_pos_ = { 0.0f,0.0f };
	float attack_charge_ = 0.0f;
	//�ړ����x
	float movespeed_ = 0.09f;
	//�v���C���[�ƓG�̋���
	float length_ = 3.0f;
	float limit_length_ = 1.2f;
	//����ł��邩
	bool dead_flag_ = false;
	//�G�ƃv���C���[�̋���
	float origin_distance_;
	float originhead_distance_;
	float distance_ = 60.0f;
	float head_distance_ = 30.0f;
	//�G�̃��[�V�����p
	float purse_positiverot_ = 180;
	//�ό`�p�̃t���O
	float defomation_count_ = 0.0f;
	//�U��
	float vibration_ = 0.0f;
	bool vibrationchange_flag_ = false;
	float atttack_timer_ = 0.f;
	float timer_limit_ = 0.f;
	//�����Ǐ]��ɕʂ̓G�����邩���Ȃ���
	bool wait_flag_ = false;
	//�����Ă����v���C���[�̏��
	Player* player_;
	XMFLOAT2 player_pos_{};
	int player_hp_ = 0;
	bool particle_flag_ = true;
};

