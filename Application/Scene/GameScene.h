#pragma once
#include<fstream>
#include<sstream>
#include<memory>
#include"BaseScene.h"
#include"BossEnemy/BossEnemy.h"
#include"Enemy.h"
#include"Player.h"
#include"TitleScene.h"
#include"RailCamera.h"

const int BUILS = 16;

enum  class GamePhase {
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


class GameScene :public BaseScene
{
public:
	


private:
	static void (GameScene::* MoveFuncTable[])();
public://�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="sceneManager_"></param>
	GameScene(SceneManager* sceneManager_);
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="dxComon"></param>
	void Initialize(DirectXCommon* dxComon) override;
	/// <summary>
	/// �X�e�[�^�X�Z�b�g
	/// </summary>
	void StatusSet();
	/// <summary>
	/// �I�u�W�F�N�g���̑S�Ă̍X�V����
	/// </summary>
	void AllUpdata();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;
	/// <summary>
	/// �I�u�W�F�N�g�̕`��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void ObjDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// �X�v���C�g�̕`��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void SpriteDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// ImgUI�`��
	/// </summary>
	void ImgDraw();
	/// <summary>
	/// �|�X�g�G�t�F�N�g�̕`��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void PostEffectDraw(DirectXCommon* dxCommon);
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon">dxCommon����R�}���h���X�g���Q�b�g</param>
	void Draw(DirectXCommon* dxCommon)override;
	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// ���X�ɖ��邭����
	/// </summary>
	void FadeIn();
	/// <summary>
	/// �X�|�b�g���C�g�̓���
	/// </summary>
	void SpotLightMove();
	/// <summary>
	/// �U����H������Ƃ��̏���
	/// </summary>
	void DamageProcess();
	/// <summary>
	/// �Q�[���I�[�o�[���̏���
	/// </summary>
	void GameOverProcess();
	/// <summary>
	/// �Q�[���N���A���̏���
	/// </summary>
	void GameClearProcesss();
	/// <summary>
	/// csv�ǂݍ���
	/// </summary>
	void LoadEnemyPopData();
	/// <summary>
	/// �G�̐���
	/// </summary>
	void UpdataEnemyPopCommands();
	/// <summary>
	/// �G���m�̒ǔ��悪��������̊m�F�Ƃ��̏ꍇ�̏���
	/// </summary>
	void CheckSameTrackPosition();


	void StartCameraWork();

	void PlayerMove();


	void CheckcCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY, bool& CheckFlag);

public:
#pragma region �֐��e�[�u��

	/// <summary>
	/// ��1�n�_���
	/// </summary>
	void MoveStartBack();
	/// <summary>
	/// ��1�n�_�O
	/// </summary>
	void MoveStartFront();

	void MovePointA();

	void MovePointALeft();

	void MovePointB();

	void MovePointC();

	void MovePointCOblique();

	void MovePointCFront();

	void GoalPointBack();

	void GoalPoint();


#pragma endregion

private://�����o�ϐ�
	//Obj
	unique_ptr <Object3d> Sphere;
	unique_ptr <Object3d> World;
	unique_ptr<Object3d> Start;
	unique_ptr<Object3d> BuilsHighAlpha[BUILS];
	unique_ptr<Object3d> BuilsLowAlpha[BUILS];
	unique_ptr<Object3d> FieldBuils[5];
	unique_ptr<Object3d> Heri;
	unique_ptr<Object3d> Goal;
	unique_ptr<Object3d> Hane;
	//�X�v���C�g
	unique_ptr<Sprite> Clear = nullptr;
	unique_ptr<Sprite> Conteniu = nullptr;
	unique_ptr<Sprite> DamageEfectSp = nullptr;
	unique_ptr<Sprite> Shot = nullptr;
	unique_ptr<Sprite> ReticleForGameOver = nullptr;
	unique_ptr<Sprite> Yes = nullptr;
	unique_ptr<Sprite> No = nullptr;
	unique_ptr<Sprite> LifeCount[5];
	unique_ptr<Sprite> Hart;
	unique_ptr<Sprite> CurtainUp;
	unique_ptr<Sprite> CurtainDown;
	unique_ptr<Sprite> Skip;
	//�v���C���[�ƓG
	list<unique_ptr<Enemy>>Robot;
	list<unique_ptr<BossEnemy>>Boss;
	unique_ptr<Player> Hero;
	stringstream EnemyPopCommands;
	//���̑��̋@�\
	unique_ptr <Audio> Bgm = nullptr;
	unique_ptr <Audio> heriFry = nullptr;
	unique_ptr <PostEffect> postEffect = nullptr;
	unique_ptr<Light> light = nullptr;
	unique_ptr<LightGroup> lightGroupe = nullptr;
	unique_ptr<Camera> GameCamera = nullptr;
	unique_ptr<RailCamera> railcamera_ = nullptr;
	//�ŏ��̃r���̃X�e�[�^�X
	XMVECTOR StartPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 StartScl = { 15.0f,15.0f,15.0f };
	XMFLOAT3 StartRot = { 0.0f,180.0f,0.0f };
	//�V���̃X�e�[�^�X
	XMVECTOR SpherePos = { 0,0,0 };
	XMFLOAT3 SphereScl = { 4.0f,4.0f,4.0f };
	XMFLOAT3 SphereRot = { 0,90,0 };
	//�t�B�[���h�̃X�e�[�^�X
	XMFLOAT3 FieldBuilscl = { 6.0f,10.0f,7.0f };
	XMVECTOR FieldBillPos[5];
	XMFLOAT3 FieldBillRot[5];
	//����̃X�e�[�^�X
	XMVECTOR WorldPos = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 WorldScl = { 100,100,100 };
	//�r���Q�̃X�e�[�^�X
	XMFLOAT3 BuilsScl = { 10.0f,10.0f,10.0f };
	XMVECTOR BuilsHighAlphaPos = { 0.0f,0.0f,-16.5f };
	XMVECTOR BuilsLowAlphaPos = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 BuilsRot = { 0.0f,90.0f,0.0f };
	//�w���̃X�e�[�^�X(�X�^�[�g�̃��[�r�[�ƏI���̃��[�r�[)
	//�X�^�[�g
	XMVECTOR Heripos = { 0.0f,75.0f,-160.0f };
	XMFLOAT3 Heriscl = { 10.0f,10.0f,10.0f };
	//�I���
	XMVECTOR GoalPos = { 56.f,3.0f,92.0f };
	XMFLOAT3 GoalScl = { 3.0f,3.0f,3.0f };
	//���̑��̕ϐ�
	XMFLOAT4 PostCol = { -1.0f,-1.0f,-1.0f,1.0f };
	int Patern = 0;
	int PlayerHp;
	int OldHp;
	bool PostEffectOnFlag = false;
	bool ClearFlag = false;
	bool StopUpdateFlag = false;
	bool DethFlag = false;
	bool GameStartFlag = false;
	bool DamageHitFlag = false;
	XMFLOAT4 DamageEfectColor = { 1.f,1.f,1.f,1.f };


	XMFLOAT3 FieldSpotLightDir = { 0,-10,0 };
	XMFLOAT3 FieldSpotLightPos = { 0,505,50 };
	XMFLOAT3 FieldSpotLightColor = { 0.9f,0.5f,0.f };
	XMFLOAT3 FieldSpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 FieldSpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 PlayerSpotLightDir = { 0,-20,0 };
	XMFLOAT3 PlayerSpotLightPos = { 0,35,0 };
	XMFLOAT3 PlayerSpotLightColor = { 1.f,1.f,1.f };
	XMFLOAT3 PlayerSpotLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 PlayerSpotLightFactorAngle = { 20.0f,30.0f };

	XMFLOAT3 SearchLightDir[3];
	XMFLOAT3 SearchLightPos[3];
	XMFLOAT3 SearchLightColor[3];
	XMFLOAT3 SearchLightAtten = { 0.f,0.f,0.f };
	XMFLOAT2 SearchLightFactorAngle = { 20.0f,30.0f };

	bool SpotLightPositionChange = false;
	bool LightPositionChangeX[2];
	bool LightPositionChangeZ[2];

	float duration = 1.f;
	float time = -1.0f;
	float value = 0.f;
	float StartPointZ = 50.f;
	float EndPointZ = 0.f;
	float StartPointX = -30;
	float EndPointX = 30;
	float EbdPointZ2 = 90.0f;
	float StartPointZ2 = 50.f;

	bool Easing = true;
	float EasingWaitTimeR = 0.f;
	bool EasingChange = false;

	XMFLOAT3 SearchLightAddPos = { 0.1f,-0.1f,0.05f };
	float LightAddPosChangeTimer = 0.0f;
	bool ChangeTimerFlag = false;

	float StartColor = -0.5f;
	float EndColor = 0.0f;
	float ColorTime = 1.0f;

	float StartColorRed = 0.0f;
	float EndColorRed = 0.8f;
	float ColorTimeRed = 0.0f;

	float LightDirEasingTime = 0.f;
	bool LightDirEasingChane = false;

	XMFLOAT2 ReticlePosition{ 0.f,0.f };
	XMFLOAT4 ReticleColor{ 1.f,1.f,1.f,1.f };
	XMFLOAT2 SpriteAnchorPoint{ 0.5f,0.5f };
	XMFLOAT2 HartPosition{ 10.f,10.f };

	XMFLOAT2 YesPosition{ WinApp::window_width / 2 - 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 NoPosition{ WinApp::window_width / 2 + 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 SpriteSize{ 240.f,240.f };
	XMFLOAT4 YesColor{ 1.f,1.f,1.f,1.f };
	XMFLOAT4 NoColor{ 1.f,1.f,1.f,1.f };
	bool YesCursorInFlag = false;
	bool NoCursorInFlag = false;


#pragma region Acrtor���玝���Ă���ϐ�(����Ȃ����̂͑S������)


	float HeriY = 0.0f;
	bool MoveFlag = false;
	bool BackObjFlag = true;
	float HeriX = 3.1f;

	bool CountFlag = false;
	//�ҋ@�R�}���h
	bool WaitFlag = false;
	int WaitT = 0;

	XMVECTOR Velo;

	bool StartMovieFlag = false;

	bool StopFlag = false;

	bool GetCamWorkFlag = false;

	XMFLOAT2 HartSize = { 64,64 };
	XMFLOAT2 MotValue = { 0.f,0.f };
	bool ReversFlag = true;
	float EasingTimer = 0.0f;
	float AddTimer = 0.01f;
	bool otherenemyarive = true;

#pragma endregion

#pragma region �v���C���[����ڍs
	//�n�܂�ƏI���̉��o�g�������J�[�e��
	XMFLOAT2 CurtainUpPos = { 0.0f,0.0f };
	XMFLOAT2 CurtainSize = { 1280.0f,100.0f };
	XMFLOAT2 CurtainDownPos = { 0.0f,620.0f };
	//Skip�����̕\��
	XMFLOAT2 SkipPos = { 0.0f,620.0f };

	XMVECTOR velocity_;
	XMFLOAT3 eyerot_{ 0.f,0.f,0.f };

	bool startflag_ = false;
	bool stanbyflag_ = false;
	int actioncount_ = 0;
	float actiontimer_ = 0.f;
	bool movieflag_ = false;
	XMVECTOR l_reticlepos;
	XMVECTOR cameravector_{ 0.f,0.f,0.f };
	float movespeed_ = 0.f;
	float changerotation_ = 0.f;
	bool FringFlag = false;
#pragma endregion


};

