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

const int BULL = 9;

class Player
{
public:
	~Player();
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="came">�J����</param>
	void Initalize(Camera* came);
	/// <summary>
	/// �X�e�[�^�X��Set�͑S������
	/// </summary>
	/// <param name="came"></param>
	void Set(Camera* camera);
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="bull">�v���C���[�̒e</param>
	/// <param name="Remaining">�c�e��</param>
	/// <param name="enePos">�G�̍��W</param>
	/// <param name="came">�J����</param>
	/// <param name="Ene2dPos">�G��2D���W</param>
	/// <param name="pat">���݂̃t�F�C�Y</param>
	void Update(Bullet* bull[], int& Remaining,const XMVECTOR enePos[], Camera* came,const XMFLOAT2 Ene2dPos[],int patern);

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
	/// <param name="enePos">�G�̍��W</param>
	/// <param name="came">�J����</param>
	/// <param name="Ene2dPos">�G��2D���W</param>
	void MouthContoroll(const XMVECTOR enePos[], Camera* came, const XMFLOAT2 Ene2dPos[]);
	//�p�[�e�B�N������
	void ParticleEfect();

#pragma region Get
	const bool& GetFinish() { return Finish; }
	//�O�������W
	const XMVECTOR& GetPosition() { return Ret_Pos; }
	//�p�x
	const XMFLOAT3& GetRotation() { return Ret_Rot; }
	//�X�P�[��
	const XMFLOAT3& GetScl() { return Ret_Scl; }
	//�s��
	const XMMATRIX& GetMat() { return Track_Mat; }
	//�񎟌����W
	const XMFLOAT2& GetRetPosition() { return Ret_Pos2D; }
	//�񎟌��X�P�[��
	const XMFLOAT2& GetRetSiz() { return Ret_Siz; }

	const bool& GetCamWork() { return CamWork; }

	const bool& GetFring() { return Fring_F; }
	//Hp
	int GetHp() { return Hp; }
#pragma endregion

#pragma region Set
	void SetPosition(const XMVECTOR& position) { this->Ret_Pos = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->Ret_Rot = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->Ret_Scl = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->Ret_Pos2D = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->Ret_Siz = scale; }
	void SetHp(int HP) { this->Hp = HP; }
	void SetFinish(const bool& finish) { this->Finish = finish; }
#pragma endregion
private:

	//Obj���f��
	ObjModel* TrackModel = nullptr;
	ObjModel* GunModel = nullptr;
	ObjModel* PlayerModel = nullptr;
	//Obj
	std::unique_ptr<Object3d> Track;
	std::unique_ptr<Object3d> Gun;
	std::unique_ptr<Object3d> Player;
	//�X�v���C�g
	std::unique_ptr<Sprite> SpriteRet;
	std::unique_ptr<Sprite> Curtain_Up;
	std::unique_ptr<Sprite> Curtain_Down;
	std::unique_ptr<Sprite> Window;
	std::unique_ptr<Sprite> Skip;
	//���C���̃G�t�F�N�g
	ParticleManager* PartGreen = nullptr;
	ParticleManager* PartRed = nullptr;
	//���̑��̋@�\
	Audio* Shot_SE;
	Audio* Reload_SE;
	RailCamera* Railcam;
	//���[�J��
		//���e�B�N��Obj�̃X�e�[�^�X
		XMVECTOR Ret_Pos = { 0.0f,55.0f,0.0f };
		XMFLOAT3 Ret_Rot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 Ret_Scl = { 0.3f,0.3f,0.3f };
		//���ˑ�Obj�̃X�e�[�^�X
		XMVECTOR Gun_Pos = { 0.0f,0.0f,-3.0f };
		XMFLOAT3 Gun_Rot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 Gun_Scl={0.3f,0.3f,0.3f};
		//�v���C���[Obj�̃X�e�[�^�X
		XMVECTOR Player_Pos = { 0.0f,0.0f,0.0f };
		XMFLOAT3 Player_Rot = { 0.0f,0.0f,0.0f };
		XMFLOAT3 Player_Scl = { 0.3f,0.3f,0.3f };
	//���[���h
		//���e�B�N���̃X�e�[�^�X
		XMVECTOR TrackWorld_Pos = { 0.0f,0.0f,0.0f };
		XMMATRIX Track_Mat;
		XMVECTOR WorldFarPos={0.0f,0.0f,0.0f};
		//���ˑ�̃X�e�[�^�X
		XMVECTOR GunWorld_Pos = { 0.0f,0.0f,-5.0f };
		XMMATRIX Gun_Mat;
		//�v���C���[�̃X�e�[�^�X
		XMVECTOR PlayerWorld_Pos = { 0.0f,0.0f,-0.1f };
		XMMATRIX Player_Mat;
		XMVECTOR RetWorld_Pos;
		//�p�[�e�B�N���̃X�e�[�^�X
		XMVECTOR Part_Pos = { 0.0f,0.0f,2.0f };
	//�r���[�|�[�g
	XMMATRIX MatViewPort;
	//�J����
	XMFLOAT3 Eye_rot = {0.0f,0.0f,0.0f};
	XMFLOAT3 Target_pos = {0.0f,0.0f,0.0f};
	XMFLOAT3 Up = { 0.0f,1.0f,0.0f };
	//�X�v���C�g
		//2D���e�B�N���̃X�e�[�^�X
		XMFLOAT4 Ret_Col = { 1,1,1,1 };
		XMFLOAT2 Ret_Anc = { 0.5f,0.5f };
		XMFLOAT2 Ret_Pos2D = { 640.0f,360.0f };
		XMFLOAT2 Ret_Siz = { 64.0f,64.0f };
		//�n�܂�ƏI���̉��o�g�������J�[�e��
		XMFLOAT2 Curtain_UpPos = { 0.0f,0.0f };
		XMFLOAT2 Curtain_Siz = { 1280.0f,100.0f };
		XMFLOAT2 Curtain_DownPos = { 0.0f,620.0f };
		//Skip�����̕\��
		XMFLOAT2 Skip_Pos = { 1100,720.0f };
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

	bool CamWork = false;

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
	int EneCount = 0;

	int PaternCount = 0;

	const float  M_PI = 3.14;

	XMFLOAT3 pos;
};

