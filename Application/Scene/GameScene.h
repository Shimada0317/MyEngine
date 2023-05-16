#pragma once
#include<fstream>
#include<sstream>
#include<memory>
#include"BaseScene.h"
#include"BossEnemy/BossEnemy.h"
#include"Continue.h"
#include"NormalEnemy.h"
#include"ThrowEnemy.h"
#include"GameBackground.h"
#include"Movie.h"
#include"Player.h"
#include"TitleScene.h"
#include"RailCamera.h"
#include"CommonBackground.h"


class GameScene :public BaseScene
{
private:
	enum GamePhase {
		NONE = 0,
		START,
		FIGHT,
		MOVE,
		CONTINUE,
		CLEAR,
	};

	enum MovieSequence {
		ACTION=0,
		TURNAROUND,
		FACELOWER,
		JUMP,
		LANDING,
		FINISH,
	};

	enum ENEMYPATERN {
		NORMAL=0,
		THROW,
		BOSS,
	};

private:
	//�ړ��p�̊֐��|�C���^	
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
	/// �n�܂�̃J�������o
	/// </summary>
	void StartProcess();
	/// <summary>
	/// �ړ����̏���
	/// </summary>
	void MoveProcess();
	/// <summary>
	/// �퓬���̏���
	/// </summary>
	void FightProcess();
	/// <summary>
	/// �Q�[���I�[�o�[���̏���
	/// </summary>
	void GameOverProcess();
	/// <summary>
	/// �Q�[���N���A���̏���
	/// </summary>
	void GameClearProcesss();
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

	void KilledAllEnemy();
	/// <summary>
	/// �n�܂�̃��[�r�[�̃X�L�b�v����
	/// </summary>
	void SkipStartMovie(XMVECTOR& bodypos);
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
	unique_ptr<Object3d> heri_;
	unique_ptr<Object3d> goal_;
	unique_ptr<Object3d> hane_;
	unique_ptr<CommonBackground> common_background_;
	unique_ptr<GameBackground>game_background_;
	//�X�v���C�g
	unique_ptr<Sprite> clear_;
	unique_ptr<Sprite> damageefectsprite_;
	unique_ptr<Sprite> shot_ = nullptr;
	unique_ptr<Sprite> reticleforgameover_;
	unique_ptr<Continue> continue_screen_;
	unique_ptr<Movie> movie_;
	//�v���C���[�ƓG
	list<unique_ptr<NormalEnemy>>robot_;
	list<unique_ptr<BossEnemy>>boss_;
	list<unique_ptr<ThrowEnemy>>throw_;
	unique_ptr<Player> player_;
	stringstream enemypopcommands_;
	//���̑��̋@�\
	unique_ptr <Audio> bgm_ = nullptr;
	unique_ptr <Audio> herifry_ = nullptr;
	unique_ptr <PostEffect> posteffect_ = nullptr;
	unique_ptr<Light> light_ = nullptr;
	unique_ptr<LightGroup> lightgroupe_ = nullptr;
	unique_ptr<Camera> camera_ = nullptr;
	unique_ptr<RailCamera> railcamera_ = nullptr;
	//�r���Q�̃X�e�[�^�X
	XMFLOAT3 builsscl_ = { 10.0f,10.0f,10.0f };
	XMVECTOR builshighalphapos_ = { 0.0f,0.0f,-16.5f };
	XMVECTOR builslowalphapos_ = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 builsrot_ = { 0.0f,90.0f,0.0f };
	//�w���̃X�e�[�^�X(�X�^�[�g�̃��[�r�[�ƏI���̃��[�r�[)
	//�X�^�[�g
	XMVECTOR heripos_ = { 0.0f,75.0f,-160.0f };
	XMFLOAT3 heriscl_ = { 10.0f,10.0f,10.0f };
	//�I���
	XMVECTOR goalpos_ = { 56.f,3.0f,92.0f };
	XMFLOAT3 goalscl_ = { 3.0f,3.0f,3.0f };
	//���̑��̕ϐ�
	XMFLOAT4 postcol_ = { -1.0f,-1.0f,-1.0f,1.0f };
	int patern_ = 0;
	int playerhp_;
	int oldhp_;
	bool posteffectonflag_ = false;
	bool damagehitflag_ = false;
	XMFLOAT4 damageefectcolor_ = { 1.f,1.f,1.f,1.f };
	//�t�B�[���h���C�g�p�̕ϐ�
	XMFLOAT3 fieldspotlightdir_ = { 0,-10,0 };
	XMFLOAT3 fieldspotlightpos_ = { 0,505,50 };
	XMFLOAT3 fieldspotlightcolor_ = { 0.9f,0.5f,0.f };
	XMFLOAT3 fieldspotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 fieldspotlightfactorangle_ = { 20.0f,30.0f };
	//�v���C���[�p�̃��C�g�̕ϐ�
	XMFLOAT3 playerspotlightdir_ = { 0,-20,0 };
	XMFLOAT3 playerspotlightpos_ = { 0,35,0 };
	XMFLOAT3 playerspotlightcolor_ = { 1.f,1.f,1.f };
	XMFLOAT3 playerspotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 playerspotlightfactorangle_ = { 20.0f,30.0f };
	//�X�e�[�W���𓮂��Ă��郉�C�g�̕ϐ�
	XMFLOAT3 searchlightdir_[3];
	XMFLOAT3 searchlightpos_[3];
	XMFLOAT3 searchlightcolor_[3];
	XMFLOAT3 searchlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 searchlightfactorangle_ = { 20.0f,30.0f };

	bool spotlightpositionchange_ = false;
	float time_ = -1.0f;
	float value_ = 0.f;
	bool easing_ = true;
	float easingwaittimer_ = 0.f;
	bool easingchange_ = false;
	float colortime_ = 1.0f;
	float colortimered_ = 0.0f;
	float lightdireasingtime_ = 0.f;

	XMFLOAT2 reticleposition_{ 0.f,0.f };
	XMFLOAT4 reticlecolor_{ 1.f,1.f,1.f,1.f };
	XMFLOAT2 spriteanchorpoint_{ 0.5f,0.5f };
	XMFLOAT2 yesposition_{ WinApp::window_width / 2 - 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 noposition_{ WinApp::window_width / 2 + 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 spritesize_{ 240.f,240.f };
	XMFLOAT4 yescolor_{ 1.f,1.f,1.f,1.f };
	XMFLOAT4 nocolor_{ 1.f,1.f,1.f,1.f };
	bool yescursorinflag_ = false;
	bool nocursorinflag_ = false;
	float heriy_ = 15.0f;
	bool stopflag_ = false;
	bool otherenemyarive_ = true;
	XMVECTOR velocity_;
	XMFLOAT3 eyerot_{ 0.f,180.f,0.f };
	XMFLOAT3 passrot_{};
	int actioncount_ = 0;
	float actiontimer_ = 0.f;
	XMVECTOR l_reticlepos;
	XMVECTOR cameravector_{ 0.f,0.f,0.f };
	float movespeed_ = 0.5f;
	float changerotation_ = 0.f;
	bool FringFlag = false;

	bool shakingstartflag_ = false;
	float shake_addvalue_ = 0.f;
	//�X�e�[�^�X
	int gamestate_ = GamePhase::NONE;
	int movie_sequence_ = MovieSequence::ACTION;

	float dir_timer_X[3];
	float dir_timer_Z[3];
};

