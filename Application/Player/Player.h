#pragma once
#include"Object3d.h"
#include"Sprite.h"
#include<vector>
#include"Audio.h"
#include"HelperMath.h"
#include"ParticleManager.h"
#include"RailCamera.h"
#include<memory>
#include<list>

enum  Phase {
	LANDINGPOINT_BACK = 0,
	LANDINGPOINT_FRONT,
	MOVEDPOINT_A,
	MOVEDPOINT_A_LEFT,
	MOVEDPOINT_B,
	MOVEDPOINT_C,
	MOVEDPOINT_C_OBLIQUE,
	MOVEDPOINT_C_FRONT,
	GOALPOINT_BACK,
	GOALPOINT,

};

class Player
{
private:
	enum State {
		WAIT = 0,
		RELOAD,
		SHOT,
		MOVE,
	};
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
	/// <param name="bull">�v���C���[�̒e</param>
	/// <param name="Remaining">�c�e��</param>
	/// <param name="enePos">�G�̍��W</param>
	/// <param name="came">�J����</param>
	/// <param name="Ene2dPos">�G��2D���W</param>
	/// <param name="pat">���݂̃t�F�C�Y</param>
	void Update(Camera* camera, Phase patern,XMFLOAT3 eyerot);
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
	void GunShotProcess(Phase paterncount);
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
	void ParticleEfect(Phase paterncount);
	/// <summary>
	/// Imgui�̕`��
	/// </summary>
	void ImGuiDraw();
public:
#pragma region Get
	//�O�������W
	const XMVECTOR& GetPosition() { return reticlepos_; }
	//�p�x
	const XMFLOAT3& GetRotation() { return reticlerot_; }
	//�X�P�[��
	const XMFLOAT3& GetScl() { return reticlescl_; }
	//�񎟌����W
	const XMFLOAT2& GetRetPosition() { return reticlepos2d_; }
	//�񎟌��X�P�[��
	const XMFLOAT2& GetRetSiz() { return reticlesize_; }
	//Hp
	int GetHp() { return hp_; }
	//��������
	const bool& GetBulletShot() { return bulletshotflag_; }

	const XMVECTOR& GetBodyWorldPos() { return bodyworldpos_; }

	const XMVECTOR& GetVelocity() { return velocity_; }

	const XMMATRIX& GetBodyMatrix() { return bodymat_; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->reticlepos_ = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->reticlerot_ = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->reticlescl_ = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->reticlepos2d_ = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->reticlesize_ = scale; }
	void SetHp(int HP) { this->hp_ = HP; }
	void SetBulletShot(const bool& BulletShot_F) { this->bulletshotflag_ = BulletShot_F; }
	void SetBodyWorldPos(const XMVECTOR& worldbodypos) { this->bodyworldpos_ = worldbodypos; }
#pragma endregion
private:
	//Obj
	unique_ptr<Object3d> gun_;
	unique_ptr<Object3d> body_;
	//�X�v���C�g
	unique_ptr<Sprite> spritereticle_;
	unique_ptr<Sprite> bulletHUD[9];
	unique_ptr<Sprite> reload_;
	//���C���̃G�t�F�N�g
	unique_ptr <ParticleManager> partgreen_ = nullptr;
	unique_ptr <ParticleManager> partred_ = nullptr;
	unique_ptr <ParticleManager> partsmoke_ = nullptr;
	//���̑��̋@�\
	unique_ptr<Audio> shotse_;
	unique_ptr<Audio> reloadse_;
	//���[�J��
		//���e�B�N��Obj�̃X�e�[�^�X
	XMVECTOR reticlepos_ = { 0.0f,55.0f,0.0f };
	XMFLOAT3 reticlerot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 reticlescl_ = { 0.3f,0.3f,0.3f };
	//���ˑ�Obj�̃X�e�[�^�X
	XMVECTOR gunpos_ = { 0.0f,0.0f,-3.0f };
	XMVECTOR gunnotparentpos_ = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 gunrot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 gunscl_ = { 0.3f,0.3f,0.3f };
	//�v���C���[Obj�̃X�e�[�^�X
	XMVECTOR bodypos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodyrot_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 bodyscl_ = { 0.3f,0.3f,0.3f };
	//���[���h
		//���e�B�N���̃X�e�[�^�X
	XMVECTOR trackworldpos_ = { 0.0f,0.0f,0.0f };
	XMMATRIX trackmat_;
	XMVECTOR worldfarpos_ = { 0.0f,0.0f,0.0f };
	//���ˑ�̃X�e�[�^�X
	XMVECTOR gunworldpos_ = { 0.0f,0.0f,-5.0f };
	XMMATRIX gunmat_;
	//�v���C���[�̃X�e�[�^�X
	XMVECTOR bodyworldpos_ = { 0.0f,0.0f,-0.1f };
	XMMATRIX bodymat_;
	XMVECTOR reticleworldpos_;
	//�p�[�e�B�N���̃X�e�[�^�X
	XMVECTOR particlepos_ = { 0.0f,0.0f,2.0f };
	//�J����
	XMFLOAT3 eyerot_ = { 0.0f,180.0f,0.0f };
	XMFLOAT3 targetpos_ = { 0.0f,0.0f,0.0f };
	XMFLOAT3 up_ = { 0.0f,1.0f,0.0f };
	//�X�v���C�g
		//2D���e�B�N���̃X�e�[�^�X
	XMFLOAT4 reticlecolor_ = { 1,1,1,1 };
	XMFLOAT2 reticleancorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reticlepos2d_ = { 640.0f,360.0f };
	XMFLOAT2 reticlesize_ = { 64.0f,64.0f };
	//�n�܂�ƏI���̉��o�g�������J�[�e��
	XMFLOAT2 curtainuppos_ = { 0.0f,0.0f };
	XMFLOAT2 curtainsize_ = { 1280.0f,100.0f };
	XMFLOAT2 curtaindownpos_ = { 0.0f,620.0f };
	//Skip�����̕\��
	XMFLOAT2 skippos_ = curtaindownpos_;
	//2D���W���̎擾�p�ϐ�
	XMVECTOR offset_ = { 0,0,1.0f };

	int hp_ = 5;
	int oldhp_ = hp_;
	//Reload
	int reloadtime_ = 0;
	//�ړ�
	XMVECTOR velocity_;
	//cam
	int actioncount_ = 0;
	float actiontimer_ = 0.0f;

	bool shakeheadflag_ = true;

	XMFLOAT3 changerot_{};


	bool bulletshotflag_ = false;
	float shotcooltime_ = 0.0f;
	bool recoilflag_ = false;
	float recoiltime_ = 0.0f;

	bool mousestopflag_ = false;

	bool recoilgunflag_ = false;
	float recoverytime_ = 0.0f;
	//�����Ă�������
	int remaining_ = 0;
	int oldremaining_ = 0;

	XMFLOAT2 spritepos_[9];
	bool dropbulletflag_[9];
	float spriterot_[9];

	float time_[9];


	//Sprite�̃X�e�[�^�X
	XMFLOAT4 reloadspritecolor_ = { 1.0f,1.0f,1.0f,0.9f };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reloadspritepos_ = { WinApp::window_width / 2,210 };
	XMFLOAT2 reloadspritesize_ = { 210,140 };

	bool reversflag_ = false;

	int playerstate_ = WAIT;
};

