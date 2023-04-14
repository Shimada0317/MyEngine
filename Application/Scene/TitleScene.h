#pragma once
#include<memory>
#include"BaseScene.h"
#include"GameScene.h"

const int BUILSAMOUNT = 16;

class TitleScene :public BaseScene
{
private:
	enum OperationPage {
		DescriptionPage = 0,
		EnemyOverViewPage,
		GameStartPrepartionPage,
	};

private:
	static const int debugTextNumber = 0;

public://�����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="sceneManager_"></param>
	TitleScene(SceneManager* sceneManager_);

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
	/// Obj����Update�͂��̒���
	/// </summary>
	void AllUpdate();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

	/// <summary>
	/// �J�����̈ړ�
	/// </summary>
	void CameraDirection();

	/// <summary>
	/// UI���C�[�W���O�Ŋg��k�������鏈��
	/// </summary>
	void UiEasingProcess();

	/// <summary>
	/// �J�[�\�����͈͓��ɓ����Ă��邩
	/// </summary>
	/// <param name="cursorPos">�J�[�\���̍��W</param>
	/// <param name="checkPos">������X�v���C�g�̍��W</param>
	/// <param name="radX">�X�v���C�g�̉��͈̔�</param>
	/// <param name="radY">�X�v���C�g�̏c�͈̔�</param>
	/// <param name="CheckFlag">�͈͓��ɓ����Ă���ꍇ�t���O��true�ɂ���</param>
	void CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY,bool& CheckFlag);

	/// <summary>
	/// ���̃X�v���C�g�͈̔�
	/// </summary>
	/// <param name="cursorPos">�J�[�\���̍��W</param>
	/// <param name="checkPos">������X�v���C�g�̍��W</param>
	/// <param name="radX">�X�v���C�g�̉��͈̔�</param>
	/// <param name="radY">�X�v���C�g�̏c�͈̔�</param>
	/// <returns></returns>
	bool NextorBack(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY);

	/// <summary>
	/// ������ʂōs���鏈��
	/// </summary>
	void DescriptionPageProces();

	/// <summary>
	///�@�t�F�[�h�A�E�g��ɃQ�[���V�[���փ`�F���W
	/// </summary>
	void FadeOutAndSceneChange();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="dxCommon"></param>
	void Draw(DirectXCommon* dxCommon)override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

private:
	//Obj
	unique_ptr <Object3d> sphere_;
	unique_ptr<Object3d> builshighalpha_[BUILSAMOUNT];
	unique_ptr<Object3d> builslowalpha_[BUILSAMOUNT];
	unique_ptr <Object3d> groundobj_;
	unique_ptr <Object3d >world_;
	unique_ptr<Object3d> start_;
	//�X�v���C�g
	unique_ptr<Sprite> title_ = nullptr;
	unique_ptr<Sprite> cursor_ = nullptr;
	unique_ptr<Sprite> clickbefore_ = nullptr;
	unique_ptr<Sprite> clickafter_ = nullptr;
	unique_ptr<Sprite> signalbefore_ = nullptr;
	unique_ptr<Sprite> signalafter_ = nullptr;
	unique_ptr<Sprite> descriptionoperation_ = nullptr;
	unique_ptr<Sprite> enemyoverview_ = nullptr;
	unique_ptr<Sprite> gamestartpreparation_ = nullptr;
	unique_ptr<Sprite> arrowright_ = nullptr;
	unique_ptr<Sprite> arrowleft_ = nullptr;
	unique_ptr<Sprite> arrowrighttrue_ = nullptr;
	unique_ptr<Sprite> arrowlefttrue_ = nullptr;
	//���̑��̋@�\
	unique_ptr<Light> light_ = nullptr;
	unique_ptr<LightGroup> lightgroupe_ = nullptr;
	unique_ptr<PostEffect> posteffct_ = nullptr;
	unique_ptr<Audio> clickse_ = nullptr;
	unique_ptr<Audio> morsecodese_ = nullptr;
	unique_ptr<Audio> bgm_ = nullptr;
	unique_ptr<Camera> titlecamera_ = nullptr;
	//���̂̃X�e�[�^�X
	XMVECTOR spherepos_ = { 0,0,0 };
	XMFLOAT3 spherescl_ = { 4.0f,4.0f,4.0f };
	XMFLOAT3 sphererot_ = { 0,0,0 };
	//�t�B�[���h�̃X�e�[�^�X
	XMVECTOR groundpos_ = { 22.5f,-1,40 };
	XMFLOAT3 groundscl_ = { 13,20,16 };
	XMFLOAT3 groundrot_ = { 0,0,0 };
	//�r���Q�̃X�e�[�^�X
	XMFLOAT3 builsscl_ = { 10.0f,10.0f,10.0f };
	XMVECTOR builshighalphapos_ = { 0.0f,0.0f,-16.5f };
	XMVECTOR builslowalphapos_ = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 builsrot_ = { 0.0f,90.0f,0.0f };
	//�J�����̈ړ���̃X�e�[�^�X
	XMVECTOR startpos_ = { 0.0f,0.0f,-16.5f };
	XMFLOAT3 startscl_ = { 15.0f,15.0f,15.0f };
	//�J�����̃X�e�[�^�X
	XMVECTOR cameramove_ = { 0.0f,0.0f,0.0f };
	XMVECTOR cameraeyemove_ = { 0.0f,0.0f,0.0f };
	bool cameraeyemoveflag_ = false;
	bool camerachangeflag_ = false;
	//����̃X�e�[�^�X
	XMVECTOR worldpos_ = { 0.0f,-1.1f,0.0f };
	XMFLOAT3 worldscl_ = { 100,100,100 };
	//�X�v���C�g�̃X�e�[�^�X
	XMFLOAT4 spritecol_ = { 1,1,1,1 };
	XMFLOAT2 anchorpoint_ = { 0.5f,0.5f };
	XMFLOAT2 reticlepos_ = { 640.0f,360.0f };
	XMFLOAT2 clickpos_ = { WinApp::window_width/2,600 };
	//�^�C�g���X�v���C�g�̃X�e�[�^�X
	XMFLOAT2 titlesize_ = { 1280.0f,720.0f };
	XMFLOAT2 titlepos_ = { 0.0f,0.0f };
	bool titlespriteflag_ = true;
	//�����̖����W
	XMFLOAT2 arrowrightpos_ = { 1240.0f,300 };
	XMFLOAT2 arrowleftpos_ = { 35.0f,300 };
	XMFLOAT2 arrowsize_ = { 32.f,32.f};
	XMFLOAT4 arrowrightcolor_ = { 1.f,1.f,1.f,1.f };
	XMFLOAT4 arrowleftcolor_ = { 1.f,1.f,1.f,1.f };
	bool righttrueinflag_ = false;
	bool lefttrueinflag_= false;
	//�|�X�g�G�t�F�N�g�̐F
	XMFLOAT4 postefectcolor_ = { 0.0f,0.0f,0.0f,1.0f };
	//�J�[�\���������Ă��邩
	bool signalcurorinflag_ = false;
	//�t�F�[�h���|���邩�|���Ȃ���
	bool fadeoutflag_ = false;
	//�N���b�N���Ă��邩�ǂ���
	bool clickflag_ = true;
	//����
	float volume_ = 0.8f;
	//�����y�[�W
	int descriptionpage_ = 0;

	float pointlightpos_[3] = { 0.f,0.f,0.f };
	float pointlightcolor_[3] = { 1.f,1.f,1.f };
	float pointlightatten_[3] = { 0.3f,0.1f,0.1f };


	XMFLOAT3 spotlightdir_ = { 0,-1,0 };
	XMFLOAT3 spotlightpos_ = { 0,1005,0 };
	XMFLOAT3 spotlightcolor_ = { 1,1,1 };
	XMFLOAT3 spotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 spotlightfactorangle_ = { 20.0f,30.0f };

	XMFLOAT3 spotlightdir2_ = { 0,0,0 };
	XMFLOAT3 spotlightpos2_ = { 10,1005,160 };
	XMFLOAT3 spotlightcolor2_ = { 0,0,1 };
	XMFLOAT3 spotlightatten2_ = { 0.f,0.f,0.f };
	XMFLOAT2 spotlightfactorangle2_ = { 20.0f,30.0f };

	float easingtimer_ = 0.f;
	bool easingchangeflag_=false;
	XMFLOAT2 clicksize_ = { 550.f,60.f };
};

