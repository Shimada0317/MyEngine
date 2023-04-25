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


class GameScene :public BaseScene
{
private:
	enum GamePhase {
		MOVIE = 0,
		FIGHT,
		MOVE,
	};

	enum ShakeScreenPatern {
		NONE=0,
		DAMAGE,
		SHOT,
	};

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

	void KilledAllEnemy();

	/// <summary>
	/// �n�܂�̃J�������o
	/// </summary>
	void StartCameraWork();
	/// <summary>
	/// �n�܂�̃��[�r�[�̃X�L�b�v����
	/// </summary>
	void SkipStartMovie();

	/// <summary>
	/// �v���C���[�̈ړ�
	/// </summary>
	void PlayerMove();
	/// <summary>
	/// ��ʗh��
	/// </summary>
	/// <param name="limitshakevalue">�V�F�C�N����l</param>
	void ScreenShake(float shakevalue, float shakingtime);
	/// <summary>
	/// �ړ����ɃJ�����h��
	/// </summary>
	void MoveShakingHead();
	/// <summary>
	/// �n�[�g�̌ۓ��̓���
	/// </summary>
	void HeartBeat();
	/// <summary>
	/// �f��̂悤�ȃ��[�r�[�̏���
	/// </summary>
	void MovieProcess();

	/// <summary>
	/// �J�[�\���ɓ����Ă��邩
	/// </summary>
	/// <param name="cursor_Pos"></param>
	/// <param name="check_Pos"></param>
	/// <param name="radX"></param>
	/// <param name="radY"></param>
	/// <param name="CheckFlag"></param>
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
	unique_ptr <Object3d> sphere_;
	unique_ptr <Object3d> world_;
	unique_ptr<Object3d> start_;
	unique_ptr<Object3d> builshighalpha_[BUILS];
	unique_ptr<Object3d> builslowalpha_[BUILS];
	unique_ptr<Object3d> fieldbuils_[5];
	unique_ptr<Object3d> heri_;
	unique_ptr<Object3d> goal_;
	unique_ptr<Object3d> hane_;
	//�X�v���C�g
	unique_ptr<Sprite> clear_;
	unique_ptr<Sprite> conteniu_;
	unique_ptr<Sprite> damageefectsprite_;
	unique_ptr<Sprite> shot_ = nullptr;
	unique_ptr<Sprite> reticleforgameover_;
	unique_ptr<Sprite> yes_;
	unique_ptr<Sprite> no_;
	unique_ptr<Sprite> lifecount_[5];
	unique_ptr<Sprite> hart_;
	unique_ptr<Sprite> curtainup_;
	unique_ptr<Sprite> curtaindown_;
	unique_ptr<Sprite> skip_;
	//�v���C���[�ƓG
	list<unique_ptr<Enemy>>robot_;
	list<unique_ptr<BossEnemy>>boss_;
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
	//�ŏ��̃r���̃X�e�[�^�X
	XMVECTOR startpos_ = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 startscl_ = { 15.0f,15.0f,15.0f };
	XMFLOAT3 startrot_ = { 0.0f,180.0f,0.0f };
	//�V���̃X�e�[�^�X
	XMVECTOR spherepos_ = { 0,0,0 };
	XMFLOAT3 spherescl_ = { 4.0f,4.0f,4.0f };
	XMFLOAT3 sphererot_ = { 0,90,0 };
	//�t�B�[���h�̃X�e�[�^�X
	XMFLOAT3 fieldbuilscl_ = { 6.0f,10.0f,7.0f };
	XMVECTOR fieldbillpos_[5];
	XMFLOAT3 fieldbillrot_[5];
	//����̃X�e�[�^�X
	XMVECTOR worldpos_ = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 worldscl_ = { 100,100,100 };
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
	bool clearflag_ = false;
	bool stopupdateflag_ = false;
	bool dethflag_ = false;
	bool gamestartflag_ = false;
	bool damagehitflag_ = false;
	XMFLOAT4 damageefectcolor_ = { 1.f,1.f,1.f,1.f };


	XMFLOAT3 fieldspotlightdir_ = { 0,-10,0 };
	XMFLOAT3 fieldspotlightpos_ = { 0,505,50 };
	XMFLOAT3 fieldspotlightcolor_ = { 0.9f,0.5f,0.f };
	XMFLOAT3 fieldspotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 fieldspotlightfactorangle_ = { 20.0f,30.0f };

	XMFLOAT3 playerspotlightdir_ = { 0,-20,0 };
	XMFLOAT3 playerspotlightpos_ = { 0,35,0 };
	XMFLOAT3 playerspotlightcolor_ = { 1.f,1.f,1.f };
	XMFLOAT3 playerspotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 playerspotlightfactorangle_ = { 20.0f,30.0f };

	XMFLOAT3 searchlightdir_[3];
	XMFLOAT3 searchlightpos_[3];
	XMFLOAT3 searchlightcolor_[3];
	XMFLOAT3 searchlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 searchlightfactorangle_ = { 20.0f,30.0f };

	bool spotlightpositionchange_ = false;
	bool lightpositionchangex_[2];
	bool lightpositionchangez_[2];

	float duration_ = 1.f;
	float time_ = -1.0f;
	float value_ = 0.f;
	float startpointz_ = 50.f;
	float endpointz_ = 0.f;
	float startpointx_ = -30;
	float endpointx_ = 30;
	float endpointz2_ = 90.0f;
	float startpointz2_ = 50.f;

	bool easing_ = true;
	float easingwaittimer_ = 0.f;
	bool easingchange_ = false;

	XMFLOAT3 searchlightaddpos_ = { 0.1f,-0.1f,0.05f };
	float lightaddposchangetimer_ = 0.0f;
	bool changetimerflag_ = false;

	float startcolor_ = -0.5f;
	float endcolor_ = 0.0f;
	float colortime_ = 1.0f;

	float startcolorred_ = 0.0f;
	float endcolorred_ = 0.8f;
	float colortimered_ = 0.0f;

	float lightdireasingtime_ = 0.f;
	bool lightdireasingchange_ = false;

	XMFLOAT2 reticleposition_{ 0.f,0.f };
	XMFLOAT4 reticlecolor_{ 1.f,1.f,1.f,1.f };
	XMFLOAT2 spriteanchorpoint_{ 0.5f,0.5f };
	XMFLOAT2 hartposition_{ 10.f,10.f };

	XMFLOAT2 yesposition_{ WinApp::window_width / 2 - 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 noposition_{ WinApp::window_width / 2 + 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 spritesize_{ 240.f,240.f };
	XMFLOAT4 yescolor_{ 1.f,1.f,1.f,1.f };
	XMFLOAT4 nocolor_{ 1.f,1.f,1.f,1.f };
	bool yescursorinflag_ = false;
	bool nocursorinflag_ = false;


#pragma region Acrtor���玝���Ă���ϐ�(����Ȃ����̂͑S������)


	float heriy_ = 15.0f;
	bool moveflag_ = false;
	bool backobjflag_ = true;
	float herix_ = 3.1f;

	bool countflag_ = false;
	//�ҋ@�R�}���h
	bool waitflag_ = false;
	int waittime_ = 0;

	bool fringflag_ = false;

	XMVECTOR velo_;

	bool startmovieflag_ = false;

	bool stopflag_ = false;

	bool getcamworkflag_ = false;

	XMFLOAT2 hartsize_ = { 64,64 };
	XMFLOAT2 originalsize_ = { 0.f,0.f };
	bool reversflag_ = true;
	float easingtimer_ = 0.0f;
	float addtimer_ = 0.01f;
	bool otherenemyarive_ = true;

#pragma endregion

#pragma region �v���C���[����ڍs
	//�n�܂�ƏI���̉��o�g�������J�[�e��
	XMFLOAT2 curtainuppos_ = { 0.0f,0.0f };
	XMFLOAT2 curtainsize_ = { 1280.0f,100.0f };
	XMFLOAT2 curtaindownpos_ = { 0.0f,620.0f };
	//Skip�����̕\��
	XMFLOAT2 skippos_ = { 0.0f,620.0f };

	XMVECTOR velocity_;
	XMFLOAT3 eyerot_{ 0.f,0.f,0.f };
	XMFLOAT3 passrot_{};
	bool startflag_ = false;
	bool stanbyflag_ = false;
	int actioncount_ = 0;
	float actiontimer_ = 0.f;
	bool movieflag_ = false;
	XMVECTOR l_reticlepos;
	XMVECTOR cameravector_{ 0.f,0.f,0.f };
	float movespeed_ = 0.5f;
	float changerotation_ = 0.f;
	bool FringFlag = false;
#pragma endregion

	int gamestate_ = MOVIE;
	bool shakingstartflag_ = false;
	float shakelimittime_ = 0.f;
	bool shakingscreenflag_ = false;
	float shakingscreenvalue_= 0.f;
	bool shake_ = true;

	int screenshakestate_ = NONE;
};

