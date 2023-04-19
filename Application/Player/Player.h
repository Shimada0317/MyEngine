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

enum  Phase{
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
	void StatusSet(Camera* camera);
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
	void Update(Camera* camera, Phase patern);
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
	/// Imgui�̕`��
	/// </summary>
	void ImGuiDraw();
	/// <summary>
	/// SE�Ăяo��
	/// </summary>
	void SoundEffect();
	/// <summary>
	/// �}�E�X�̎擾
	/// </summary>
	void MouseContoroll();
	/// <summary>
	/// ��ʗh��
	/// </summary>
	/// <param name="limitshakevalue">�V�F�C�N����l</param>
	void ScreenShake(float shakevalue, float shakingtime);
	/// <summary>
	/// �_���[�W�H������Ƃ��̏���
	/// </summary>
	void DamageProcess();
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
public:
#pragma region Get
	//�O�������W
	const XMVECTOR& GetPosition() { return ReticlePos; }
	//�p�x
	const XMFLOAT3& GetRotation() { return ReticleRot; }
	//�X�P�[��
	const XMFLOAT3& GetScl() { return ReticleScl; }
	//�񎟌����W
	const XMFLOAT2& GetRetPosition() { return ReticlePos2D; }
	//�񎟌��X�P�[��
	const XMFLOAT2& GetRetSiz() { return ReticleSize; }
	//Hp
	int GetHp() { return Hp; }
	//��������
	const bool& GetBulletShot() { return BulletShotFlag; }

	const XMVECTOR& GetBodyWorldPos() { return BodyWorldPos; }
	
	const XMVECTOR& GetVelocity() { return Velocity; }

	const XMMATRIX& GetBodyMatrix() { return BodyMat; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->ReticlePos = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->ReticleRot = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->ReticleScl = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->ReticlePos2D = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->ReticleSize = scale; }
	void SetHp(int HP) { this->Hp = HP; }
	void SetBulletShot(const bool& BulletShot_F) { this->BulletShotFlag = BulletShot_F; }
	void SetBodyWorldPos(const XMVECTOR& worldbodypos) { this->BodyWorldPos = worldbodypos; }
#pragma endregion
private:
	//Obj
	unique_ptr<Object3d> Track;
	unique_ptr<Object3d> Gun;
	unique_ptr<Object3d> Body;
	unique_ptr<Object3d> Cartridge;
	//�X�v���C�g
	unique_ptr<Sprite> SpriteReticle;
	unique_ptr<Sprite> CurtainUp;
	unique_ptr<Sprite> CurtainDown;
	unique_ptr<Sprite> Skip;
	unique_ptr<Sprite> Window;
	unique_ptr<Sprite> bulletHUD[9];
	unique_ptr<Sprite> Reload;
	//���C���̃G�t�F�N�g
	ParticleManager* PartGreen = nullptr;
	ParticleManager* PartRed = nullptr;
	ParticleManager* PartSmoke = nullptr;
	//���̑��̋@�\
	Audio* ShotSe;
	Audio* ReloadSe;
	RailCamera* RailCam;
	//���[�J��
		//���e�B�N��Obj�̃X�e�[�^�X
	XMVECTOR ReticlePos = { 0.0f,55.0f,0.0f };
	XMFLOAT3 ReticleRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ReticleScl = { 0.3f,0.3f,0.3f };
	//���ˑ�Obj�̃X�e�[�^�X
	XMVECTOR GunPos = { 0.0f,0.0f,-3.0f };
	XMVECTOR GunNotParentPos = { 0.0f,0.0f,-10.0f };
	XMFLOAT3 GunRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 GunScl = { 0.3f,0.3f,0.3f };
	//���Obj�X�e�[�^�X
	XMVECTOR CartridgePos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 CartridgeRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 CartridgeScl = { 0.1f,0.1f,0.1f };
	//�v���C���[Obj�̃X�e�[�^�X
	XMVECTOR BodyPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 BodyRot = { 0.0f,0.0f,0.0f };
	XMFLOAT3 BodyScl = { 0.3f,0.3f,0.3f };
	//���[���h
		//���e�B�N���̃X�e�[�^�X
	XMVECTOR TrackWorldPos = { 0.0f,0.0f,0.0f };
	XMMATRIX TrackMat;
	XMVECTOR WorldFarPos = { 0.0f,0.0f,0.0f };
	//���ˑ�̃X�e�[�^�X
	XMVECTOR GunWorldPos = { 0.0f,0.0f,-5.0f };
	XMMATRIX GunMat;
	//�v���C���[�̃X�e�[�^�X
	XMVECTOR BodyWorldPos = { 0.0f,0.0f,-0.1f };
	XMMATRIX BodyMat;
	XMVECTOR ReticleWorldPos;
	//�p�[�e�B�N���̃X�e�[�^�X
	XMVECTOR ParticlePos = { 0.0f,0.0f,2.0f };
	//�r���[�|�[�g
	XMMATRIX MatViewPort;
	//�J����
	XMFLOAT3 EyeRot = { 0.0f,180.0f,0.0f };
	XMFLOAT3 TargetPos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 Up = { 0.0f,1.0f,0.0f };
	//�X�v���C�g
		//2D���e�B�N���̃X�e�[�^�X
	XMFLOAT4 ReticleColor = { 1,1,1,1 };
	XMFLOAT2 ReticleAncorPoint = { 0.5f,0.5f };
	XMFLOAT2 ReticlePos2D = { 640.0f,360.0f };
	XMFLOAT2 ReticleSize = { 64.0f,64.0f };
	//�n�܂�ƏI���̉��o�g�������J�[�e��
	XMFLOAT2 CurtainUpPos = { 0.0f,0.0f };
	XMFLOAT2 CurtainSize = { 1280.0f,100.0f };
	XMFLOAT2 CurtainDownPos = { 0.0f,620.0f };
	//Skip�����̕\��
	XMFLOAT2 SkipPos = { 0.0f,620.0f };
	//2D���W���̎擾�p�ϐ�
	XMVECTOR offset = { 0,0,1.0f };

	int Hp = 5;
	int OldHp = 0;
	//Reload
	int ReloadTime = 0;
	int Anser = 0;
	//�ړ�
	XMVECTOR Velocity;
	//�e�̑��x
	float MoveSpeed = 0;


	//cam
	float ShakingValue = 0.0f;

	int ActionCount = 0;
	float ActionTimer = 0.0f;

	bool ShakeHeadFlag = true;

	float ChangeRot = 0;


	bool BulletShotFlag = false;
	float ShotCoolTime = 0.0f;
	bool RecoilFlag = false;
	float RecoilTime = 0.0f;
	float ShakingScreenValue = 0.0f;
	bool ShakingScreenFlag = true;
	XMFLOAT3 OldEyeRot;

	float easetimer = 0;

	bool MouseStopFlag = false;
	float ShakeLimitTime = 0;
	bool ShakingStartFlag = false;
	XMVECTOR OldPos;
	bool RecoilGunFlag = false;
	float RecoveryTime = 0.0f;
	//�����Ă�������
	int Remaining = 0;
	int OldRemaining = 0;

	XMFLOAT2 SpritePos[9];
	bool DropBulletFlag[9];
	float SpriteRot[9];

	float Time[9];


	//Sprite�̃X�e�[�^�X
	XMFLOAT4 ReloadSpriteColor = { 1.0f,1.0f,1.0f,0.9f };
	XMFLOAT2 AnchorPoint = { 0.5f,0.5f };
	XMFLOAT2 ReloadSpritePos = { WinApp::window_width/2,210 };
	XMFLOAT2 ReloadSpriteSize = { 210,140 };

	bool ReversFlag = false;

	XMFLOAT3 pos;

	XMVECTOR camvec{ 0.f,0.f,0.f };
	XMMATRIX camMat;


	int playerstate_ = WAIT;
};

