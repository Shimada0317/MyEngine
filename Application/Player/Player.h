#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include<vector>
#include"Audio.h"
#include"BulletUI.h"
#include"HelperMath.h"
#include"ParticleManager.h"
#include"PlayerUI.h"
#include"RailCamera.h"
#include<memory>
#include<list>

enum  Phase {
	kLandingPointBack = 0,
	kLandingPointFront,
	kMovedPointA,
	kMovedPointALeft,
	kMovedPointB,
	kMovedPointC,
	kMovedPointCOblique,
	kMovedPointCFront,
	kGoalPointBack,
	kGoalPoint,

};

class Player
{
private:
	enum State {
		kWait = 0,
		kReload,
		kShot,
	} player_state_;
private:
	static void (Player::* StateFunctable[])();
public:
	~Player();
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="came">�J����</param>
	void Initalize(Camera* camera);
	/// <summary>
	/// �X�e�[�^�X��Set�͑S������
	/// </summary>
	/// <param name="came"></param>
	void StatusSet(Camera* camera, XMFLOAT3 eyerot);
	/// <summary>
	///�@�I�u�W�F�N�g���̍X�V����
	/// </summary>
	void AllUpdate();
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <param name="patern">�Q�[���̃t�F�C�Y</param>
	/// <param name="eyerot">�J�����̊p�x</param>
	/// <param name="gamestate">�Q�[���̏��</param>
	/// <param name="state">�Q�[���J�n�̒l</param>
	void Update(Camera* camera, Phase patern,XMFLOAT3 eyerot,int gamestate,int state);
	/// <summary>
	/// �ҋ@���̏���
	/// </summary>
	void WaitProcess();
	/// <summary>
	/// �p�[�e�B�N���̕`��
	/// </summary>
	/// <param name="cmdeList">�R�}���h���X�g</param>
	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);
	/// <summary>
	/// �X�v���C�g�̕`��
	/// </summary>
	void SpriteDraw();
	/// <summary>
	/// Obj�̕`��
	/// </summary>
	void ObjDraw();
	/// <summary>
	/// SE�Ăяo��
	/// </summary>
	void SoundEffect();
	/// <summary>
	/// �}�E�X�̎擾
	/// </summary>
	void MouseContoroll();
	/// <summary>
	/// ���C����
	/// </summary>
	void GunShotProcess();
	/// <summary>
	/// UI�̓�������
	/// </summary>
	void UIMotionProcess();
	/// <summary>
	/// ���R�C���̏���
	/// </summary>
	void RecoilProcess();
	/// <summary>
	/// �����[�h����
	/// </summary>
	void ReloadProcess();
	/// <summary>
	/// �p�[�e�B�N������
	/// </summary>
	void ParticleEfect(int paterncount);
	/// <summary>
	/// ���X�ɑ傫������
	/// </summary>
	void SlowlyLargeHUD();
	/// <summary>
	/// ���X�ɏ���������
	/// </summary>
	void SlowlySmallHUD();
	/// <summary>
	/// Imgui�̕`��
	/// </summary>
	void ImGuiDraw();
public:
#pragma region Get
	//�O�������W
	const XMVECTOR& GetPosition() { return reticle_pos_; }
	//�p�x
	const XMFLOAT3& GetRotation() { return reticle_rot_; }
	//�X�P�[��
	const XMFLOAT3& GetScl() { return reticle_scl_; }
	//�񎟌����W
	const XMFLOAT2& GetRetPosition() { return reticle_pos2d_; }
	//�񎟌��X�P�[��
	const XMFLOAT2& GetRetSiz() { return reticle_size_; }
	//Hp
	int GetHp() { return hp_; }
	//��������
	const bool& GetBulletShot() { return bullet_shotflag_; }

	const XMVECTOR& GetBodyWorldPos() { return body_worldpos_; }

	const XMMATRIX& GetBodyMatrix() { return body_mat_; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->reticle_pos_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->reticle_rot_ = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->reticle_scl_ = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->reticle_pos2d_ = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->reticle_size_ = scale; }
	void SetHp(int HP) { this->hp_ = HP; }
	void SetBulletShot(const bool& bulletshotflag) { this->bullet_shotflag_ = bulletshotflag; }
	void SetBodyWorldPos(const XMVECTOR& worldbodypos) { this->body_worldpos_ = worldbodypos; }
#pragma endregion
private:
	//Obj
	unique_ptr<Object3d> gun_;
	unique_ptr<Object3d> body_;
	//�X�v���C�g
	unique_ptr<Sprite> sprite_reticle_;
	unique_ptr<BulletUI>bullet_ui_;
	unique_ptr<PlayerUI> player_ui_;
	//���C���̃G�t�F�N�g
	unique_ptr <ParticleManager> part_green_ = nullptr;
	unique_ptr <ParticleManager> part_red_ = nullptr;
	unique_ptr <ParticleManager> part_smoke_ = nullptr;
	//���̑��̋@�\
	unique_ptr<Audio> shot_se_;
	unique_ptr<Audio> reload_se_;
	//���[�J��
		//���e�B�N��Obj�̃X�e�[�^�X
	XMVECTOR reticle_pos_ = { 0.0f,55.0f,0.0f };
	XMFLOAT3 reticle_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 reticle_scl_ = { 0.3f,0.3f,0.3f };
	//���ˑ�Obj�̃X�e�[�^�X
	XMVECTOR gun_pos_ = { 0.0f,0.0f,-3.0f };
	XMVECTOR gun_notparentpos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 gun_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 gun_scl_ = { 0.3f,0.3f,0.3f };
	//�v���C���[Obj�̃X�e�[�^�X
	XMVECTOR body_pos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 body_rot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 body_scl_ = { 0.3f,0.3f,0.3f };
	//���[���h
		//���e�B�N���̃X�e�[�^�X
	XMVECTOR track_worldpos_ = { 0.0f,0.0f,0.0f };
	XMMATRIX track_mat_;
	XMVECTOR world_farpos_ = { 0.0f,0.0f,0.0f };
	//���ˑ�̃X�e�[�^�X
	XMVECTOR gun_worldpos_ = { 0.0f,0.0f,-5.0f };
	XMMATRIX gun_mat_;
	//�v���C���[�̃X�e�[�^�X
	XMVECTOR body_worldpos_ = { 0.0f,0.0f,-0.1f };
	XMMATRIX body_mat_;
	XMVECTOR reticle_worldpos_;
	//�p�[�e�B�N���̃X�e�[�^�X
	XMVECTOR particle_pos_ = { 0.0f,0.0f,2.0f };
	//�J����
	XMFLOAT3 eye_rot_ = { 0.0f,180.0f,0.0f };
	XMFLOAT3 target_pos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 up_ = { 0.0f,1.0f,0.0f };
	//�X�v���C�g
	//���ʂ̃A���J�[�|�C���g
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
		//2D���e�B�N���̃X�e�[�^�X
	XMFLOAT4 reticle_color_ = { 1,1,1,1 };
	XMFLOAT2 ancorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reticle_pos2d_ = { 640.0f,360.0f };
	XMFLOAT2 reticle_size_ = { 64.0f,64.0f };
	//�n�܂�ƏI���̉��o�g�������J�[�e��
	XMFLOAT2 curtain_uppos_ = { 0.0f,0.0f };
	XMFLOAT2 curtain_size_ = { 1280.0f,100.0f };
	XMFLOAT2 curtain_downpos_ = { 0.0f,620.0f };
	//Reload�̃X�e�[�^�X
	XMFLOAT4 reload_spritecolor_ = { 1.0f,1.0f,1.0f,0.9f };
	XMFLOAT2 reload_spritepos_ = { WinApp::window_width / 2,210 };
	XMFLOAT2 reload_spritesize_ = { 210,140 };
	//UI�̃X�e�[�^�X
	XMFLOAT2 ui_bulletpos{ 1220.0f,25.0f  };
	XMFLOAT2 ui_reloadpos_{ WinApp::window_width / 2,210 };
	//Skip�����̕\��
	XMFLOAT2 skippos_ = curtain_downpos_;
	//2D���W���̎擾�p�ϐ�
	XMVECTOR offset_ = { 0,0,1.0f };
	//�v���C���[��Hp
	int hp_ = 5;
	int old_hp_ = hp_;
	//Reload
	int reload_time_ = 0;
	//�c�e��
	int remaining_ = 0;
	int old_remaining_ = 0;
	//�J������eye�������A�}�Y���t���b�V���̕\���Ɏg�p
	XMFLOAT3 change_rot_{};
	//�e�̔��˂̏���
	bool bullet_shotflag_ = false;
	float shot_cooltime_ = 0.0f;
	//���R�C���̏����Ɏg�������
	float recoil_time_ = 0.0f;
	float recovery_time_ = 0.0f;
	bool recoil_gunflag_ = false;
	//UI�̑傫�������܂ōs�����Ƃ��Ƀt���O�𗧂Ă�
	bool revers_flag_ = false;
	XMFLOAT3 pos{};
	int game_phase_{};
};

