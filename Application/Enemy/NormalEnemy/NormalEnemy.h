#pragma once
#include"Audio.h"
#include"ObjParticle.h"
#include"DirectXCommon.h"
#include"Sprite.h"
#include"ParticleManager.h"
#include<DirectXMath.h>
#include<memory>
#include<list>

class Camera;
class Player;

class NormalEnemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//�f�X�g���N�^
	~NormalEnemy();
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="allRot">��]</param>
	/// <param name="allPos">���W</param>
	/// <param name="came">�J����</param>
	/// <param name="Step">�ړ����ɉ��ړ����邩</param>
	void Initialize(const XMFLOAT3& allRot, const XMVECTOR& allPos, Camera* camera, const XMVECTOR& trackpoint);
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
	void Update(Player* player);
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon);
	/// <summary>
	/// �ό`
	/// </summary>
	void Defomation();
	/// <summary>
	/// �v���C���[�ǔ����[�h
	/// </summary>
	void TrackPlayerMode();
	/// <summary>
	/// �U�����[�h
	/// </summary>
	/// <param name="playerHp">�v���C���[��Hp</param>
	void AttackMode();

	/// <summary>
	/// �U��
	/// </summary>
	/// <param name="playerhp">�v���C���[��Hp</param>
	void Attack(float& attacktimer);

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

	void WaitTrack(bool otherenemyarive);

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
	/// 2D���W�̃Q�b�^�[
	/// </summary>
	/// <returns></returns>
	const XMFLOAT2& Get2DPosition() { return rockon_pos_; }
	/// <summary>
	/// �|����Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return dead_flag_; }

	bool GetArive() { return robotarive_flag_; }

private:
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
	//�T�E���h�G�t�F�N�g
	Audio* clushse_;
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
	float movespeed_ = 0.09f;
	//�v���C���[�ƓG�̋���
	float length_ = 3.0f;
	float limit_length_ =1.5f;
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
	float vibration_=0.0f;
	bool vibrationchange_flag_ = false;
	XMVECTOR attack_beforepos_ = { 0.0f,0.0f,0.0f };
	float limit_distance_ = 0.0f;
	float atttack_timer_ = 0.f;
	float timer_limit_ = 0.f;
	bool random_flag_ = false;
	bool particleefect_flag_ = true;
	//�����Ǐ]��ɕʂ̓G�����邩���Ȃ���
	bool wait_flag_ = false;
	bool objparticle_flag_ = false;
	//�����Ă����v���C���[�̏��
	Player* player_;
	XMFLOAT2 player_pos_{};
	bool player_shot_ = false;
	int player_hp_ = 0;
};

