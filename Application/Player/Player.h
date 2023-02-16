#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Sprite.h"
#include<vector>
#include"Audio.h"
#include"Bullet.h"
#include"ParticleManager.h"
#include"RailCamera.h"

#include<memory>
#include<list>

class Player
{
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
	/// �X�V����
	/// </summary>
	/// <param name="bull">�v���C���[�̒e</param>
	/// <param name="Remaining">�c�e��</param>
	/// <param name="enePos">�G�̍��W</param>
	/// <param name="came">�J����</param>
	/// <param name="Ene2dPos">�G��2D���W</param>
	/// <param name="pat">���݂̃t�F�C�Y</param>
	void Update(int& Remaining, Camera* camera,int patern);

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
	/// �X�^�[�g���̃J�������[�N
	/// </summary>
	void CameraWork();
	/// <summary>
	/// �G��|�������̈ړ�
	/// </summary>
	/// <param name="move"></param>
	/// <param name="patern">�t�F�C�Y�ԍ�</param>
	void PlayerMove(bool& move,int patern);
	//Obj�̕`��
	void ObjDraw();
	//Imgui�̕`��
	void ImGuiDraw();
	//�r���[�|�[�g�s��̌v�Z
	void ChangeViewPort(XMMATRIX& Track_Mat);
	//SE�Ăяo��
	void SoundEffect();
	/// <summary>
	/// �}�E�X�̎擾
	/// </summary>
	void MouthContoroll();
	/// <summary>
	/// ��ʗh��
	/// </summary>
	/// <param name="limitshakevalue">�V�F�C�N����l</param>
	void ScreenShake(float shakevalue,float shakingtime);
	//�p�[�e�B�N������
	void ParticleEfect();

#pragma region Get
	const bool& GetFinish() { return Finish; }
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
	//�J�������[�N
	const bool& GetCamWork() { return CameraWork_F; }

	const bool& GetFring() { return Fring_F; }
	//Hp
	int GetHp() { return Hp; }

	const bool& GetBulletShot() { return BulletShot_F; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->ReticlePos = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->ReticleRot = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->ReticleScl = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->ReticlePos2D = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->ReticleSize = scale; }
	void SetHp(int HP) { this->Hp = HP; }
	void SetFinish(const bool& finish) { this->Finish = finish; }
	void SetBulletShot(const bool& BulletShot_F) { this->BulletShot_F = BulletShot_F; }
#pragma endregion
private:
	//Obj
	std::unique_ptr<Object3d> Track;
	std::unique_ptr<Object3d> Gun;
	std::unique_ptr<Object3d> Body;
	std::unique_ptr<Object3d> Cartridge;
	//�X�v���C�g
	std::unique_ptr<Sprite> SpriteReticle;
	std::unique_ptr<Sprite> CurtainUp;
	std::unique_ptr<Sprite> CurtainDown;
	std::unique_ptr<Sprite> Window;
	std::unique_ptr<Sprite> Skip;
	//���C���̃G�t�F�N�g
	ParticleManager* PartGreen = nullptr;
	ParticleManager* PartRed = nullptr;
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
		XMVECTOR GunPos = { 0.0f,1.0f,-12.0f };
		XMVECTOR GunNotParentPos = { 0.0f,0.0f,-10.0f };
		XMFLOAT3 GunRot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 GunScl={0.3f,0.3f,0.3f};
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
		XMVECTOR WorldFarPos={0.0f,0.0f,0.0f};
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
	XMFLOAT3 EyeRot = {0.0f,0.0f,0.0f};
	XMFLOAT3 TargetPos = {0.0f,0.0f,0.0f};
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
		XMFLOAT2 SkipPos = { 1100,720.0f };
	//2D���W���̎擾�p�ϐ�
	XMVECTOR offset = { 0,0,1.0f };

	bool Particle_F = false;
	int Hp = 5;
	//Reload
	bool ReloadFlag = false;
	int ReloadTime = 0;
	int Anser = 0;
	//�ړ�
	XMVECTOR vel;
	bool Move_F = false;
	//�e�̑��x
	float MoveSpeed=0;
	//�������Ƃ���timer
	float MoveTimer = 0.0f;
	int WaveCount = 0;

	//cam
	int shake = 0;

	bool Finish = false;

	bool CameraWork_F = false;

	bool MovieFlag = false;
	bool stanby = false;
	int ActionCount = 0;
	float ActionTimer = 0.0f;

	bool Start_F = false;

	bool Fring_F = false;

	bool ShakeHead_F = true;

	bool ReloadSound_F = true;

	float ChangeRot = 0;

	float Distance = 11;
	int EnemyCount = 0;

	int PaternCount = 0;

	const float  M_PI = 3.14;

	XMFLOAT3 pos;


	bool BulletShot_F = false;
	float ShotCoolTime = 0.0f;
	bool Recoil_F = false;
	float RecoilTime = 0.0f;
	float ShakingScreenValue = 0.0f;
	bool ShakingScreen_F = true;
	XMFLOAT3 OldEyeRot;

	float easetimer = 0;

	bool MouseStop_F = false;
	float ShakeLimitTime = 0;
	bool ShakingStart = false;
	XMVECTOR OldPos;
	bool RecoilGun = false;
	float RecoveryTime = 0.0f;
};

