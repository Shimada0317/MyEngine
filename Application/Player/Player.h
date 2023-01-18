#pragma once
#include"ObjModel.h"
#include"Object3d.h"
#include"Input.h"
#include"DebugText.h"
#include"Action.h"
#include"DebugCamera.h"
#include"Texture.h"
#include"TextureModel.h"
#include"WinApp.h"
#include<vector>
#include"Audio.h"
#include"Bullet.h"
#include"ParticleManager.h"
#include"RailCamera.h"
#include"Mouse.h"

#include<memory>
#include<list>

const int BULL = 9;

class Player
{
public:
	~Player();

	void Initalize(Camera* came);

	void Set(Camera* came);

	void Updata(Bullet* bull[], int& Remaining,const XMVECTOR enePos[], Camera* came,const XMFLOAT2 Ene2dPos[],int pat);

	void ParticleDraw(ID3D12GraphicsCommandList* cmdeList);

	void SpriteDraw();

	void CameraWork();

	void PlayerMove(bool& move,int patern);

	void ObjDraw();

	void ImGuiDraw();

	void Finalize();

	void ChangeViewPort(XMMATRIX& Track_Mat);

	void SoundEffect();

	void MouthContoroll(const XMVECTOR enePos[], Camera* came, const XMFLOAT2 Ene2dPos[]);

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
	const XMFLOAT2& GetRetPosition() { return retpos; }
	//�񎟌��X�P�[��
	const XMFLOAT2& GetRetSiz() { return retsize; }

	const bool& GetCamWork() { return CamWork; }

	const bool& GetFring() { return Fring_F; }
	//Hp
	int GetHp() { return Hp; }
#pragma endregion

#pragma region Set

	void SetPosition(const XMVECTOR& position) { this->Ret_Pos = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->Ret_Rot = rotation; }
	void SetScl(const XMFLOAT3& scale) { this->Ret_Scl = scale; }
	void SetRetPosition(const XMFLOAT2& position) { this->retpos = position; }
	void SetRetSiz(const XMFLOAT2& scale) { this->retsize = scale; }
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
	Mouse* mouse = nullptr;
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
	XMFLOAT4 spCol = { 1,1,1,1 };
	XMFLOAT2 anc = { 0.5f,0.5f };
	XMFLOAT2 retpos = { 640.0f,360.0f };
	XMFLOAT2 retsize = { 64.0f,64.0f };
	XMFLOAT2 curtainPos = { 0.0f,0.0f };
	XMFLOAT2 curtainSiz = { 1280.0f,100.0f };
	XMFLOAT2 curtainPos2 = { 0.0f,620.0f };
	XMFLOAT2 windowPos = { 0.0f,0.0f };
	XMFLOAT2 skipPos = { 1100,720.0f };
	XMVECTOR offset = { 0,0,1.0f };

	bool particle = false;
	int Hp = 5;
	//Reload
	bool ReloadFlag = false;
	int ReloadTime = 0;
	int Anser = 0;

	XMVECTOR vel;
	bool Active = false;
	float kBulletSpeed=0;
	XMVECTOR velocity = { 0.0f,0.0f,0.0f };
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

