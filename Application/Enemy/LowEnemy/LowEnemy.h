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

class LowEnemy
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	//��ԑJ�ڂ̊֐��|�C���^
	static void (LowEnemy::* StateFuncTable[])();
private:
	enum State {
		kDefomation,
		kMove,
		kDeath,
	}state_;


	enum Move {
		kNormal,
		kJump,
	}move_state_;
public:
	//�f�X�g���N�^
	~LowEnemy();
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="allRot">��]</param>
	/// <param name="allPos">���W</param>
	/// <param name="came">�J����</param>
	/// <param name="Step">�ړ����ɉ��ړ����邩</param>
	void Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint);
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
	/// ������̑ҋ@
	/// </summary>
	void WaitMode();
	/// <summary>
	/// �ҋ@��̍U��
	/// </summary>
	void AttackMode();
	/// <summary>
	/// �_���[�W��H�����
	/// </summary>
	void Damage();
	/// <summary>
	/// ����
	/// </summary>
	void Death();
	/// <summary>
	/// �W�J���̑傫��
	/// </summary>
	void DeploymentScale();
	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void ParticleEfect();
	/// <summary>
	/// �����ɂ���
	/// </summary>
	void Transparentize();
	/// <summary>
	/// �U�����󂯂��Ƃ��ɐF��ς���
	/// </summary>
	void HitColor();

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

	bool GetArive() { return robotarive_flag_; }

private:
	//Obj
	//��
	std::unique_ptr<Object3d> headpart_;
	//���r
	std::unique_ptr<Object3d> armspart_;
	//����
	std::unique_ptr<Object3d> legspart_;
	//�e
	std::unique_ptr<Object3d> shadow_;
	//���S
	std::unique_ptr<Object3d> center_;
	//Obj�p�[�e�B�N��
	std::list<std::unique_ptr<ObjParticle>>obj_particle_;
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
	int hp_ = {};
	int oldhp_ = {};
	bool robotarive_flag_ = false;
	XMVECTOR track_point_ = { 0,0,0 };
	XMVECTOR oldtrack_point_ = { 0,0,0 };
	XMVECTOR faketrack_point_ = { 0,0,0 };
	//�p�[�c���Ƃ̃X�P�[��
	XMFLOAT3 headpart_scl_ = { 0.2f,0.2f,0.2f };
	XMFLOAT3 armspart_scl_ = { 0.1f,0.1f,0.1f };
	//�p�[�c���Ƃ̃|�W�V����
	XMVECTOR headpart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR armspart_pos_ = { 0.0f,0.0f,0.0f };
	XMVECTOR legspart_pos_ = { 0.0f,0.0f,0.0f };
	//�p�[�c���Ƃ̐F
	XMFLOAT4 headpart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 armspart_color_ = { 1.0f,1.0f,1.0f,1.0f };
	//�p�[�c���Ƃ̉�]
	XMFLOAT3 headpart_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 armspart_rot_ = { 0.0f,0.0f,0.0f };
	//�p�[�c���Ƃɓn���X�e�[�^�X
	XMVECTOR all_pos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 all_rot_;
	//�e�̃X�e�[�^�X
	XMVECTOR shadow_pos_ = { 0,0,0 };
	XMFLOAT4 shadow_color_ = { 0.0f,0.0f,0.0f,0.1f };
	//�G������2D�n�̃X�e�[�^�X
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT4 rockon_color_ = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT2 rockonhead_pos_ = { 0.0f,0.0f };
	//�ړ����x
	float movespeed_ = 0.15f;
	//�v���C���[�ƓG�̋���
	float length_ = 3.0f;
	float limit_length_ = 1.5f;
	//����ł��邩
	bool dead_flag_ = false;
	//�G�ƃv���C���[�̋���
	float originhead_distance_;
	float head_distance_ = 30.0f;
	//�ό`�p�̃t���O
	float defomation_count_ = 0.0f;

	bool particleefect_flag_ = true;

	//�����Ă����v���C���[�̏��
	Player* player_;
	XMFLOAT2 player_pos_{};
	int player_hp_ = 0;
	float state_timer_ = 0.f;
	float gravity_timer_=1.f;
};

