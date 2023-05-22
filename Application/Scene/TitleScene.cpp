#include "TitleScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"Collision.h"
#include"Mouse.h"
#include"HelperMath.h"
#include"SceneManager.h"

const int MaxRemainingBullet = 9;

//�R���X�g���N�^
TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}
//����������
void TitleScene::Initialize(DirectXCommon* dxComon)
{
	const XMFLOAT4 kRegularColor = { 1.f,1.f,1.f,1.f };
	const XMFLOAT2 kDescriptionScreenPosition = { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f };

	//�J�����̐���
	titlecamera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(titlecamera_.get());
	//���C�g�̐���
	lightgroupe_ = make_unique<LightGroup>();
	lightgroupe_ = LightGroup::Create();
	//���C�g�̃Z�b�g
	Object3d::SetLightGroup(lightgroupe_.get());
	//�X�v���C�g�̐���
	title_.reset(Sprite::SpriteCreate(Name::kTitle, { 1.0f,1.0f }));
	cursor_.reset(Sprite::SpriteCreate(Name::kReticle, reticlepos_, spritecol_, anchorpoint_));
	clickbefore_.reset(Sprite::SpriteCreate(Name::kClickStartBefore, clickpos_, kRegularColor, anchorpoint_));
	clickafter_.reset(Sprite::SpriteCreate(Name::kClickStartAfter, clickpos_, kRegularColor, anchorpoint_));
	signalbefore_.reset(Sprite::SpriteCreate(Name::kSignalBefore, clickpos_, kRegularColor, anchorpoint_));
	signalafter_.reset(Sprite::SpriteCreate(Name::kSignalAfter, clickpos_, kRegularColor, anchorpoint_));
	descriptionoperation_.reset(Sprite::SpriteCreate(Name::kOperationDescription, kDescriptionScreenPosition, spritecol_, anchorpoint_));
	enemyoverview_.reset(Sprite::SpriteCreate(Name::kEnemyDescription, kDescriptionScreenPosition, spritecol_, anchorpoint_));
	gamestartpreparation_.reset(Sprite::SpriteCreate(Name::kStartScreen, kDescriptionScreenPosition, spritecol_, anchorpoint_));
	arrowright_.reset(Sprite::SpriteCreate(Name::kArrowRight, arrowrightpos_, arrowrightcolor_, anchorpoint_));
	arrowleft_.reset(Sprite::SpriteCreate(Name::kArrowLeft, arrowleftpos_, arrowleftcolor_, anchorpoint_));
	bullet_ui_ = make_unique<BulletUI>();
	bullet_ui_->Create(remaining_,ui_bulletpos_,ui_reloadpos_);
	//�I�u�W�F�N�g�̐���
	common_background_ = make_unique<CommonBackground>();
	common_background_->Initialize();
	//SE�̏�����
	clickse_ = make_unique<Audio>();
	clickse_->Initialize();
	bgm_ = make_unique<Audio>();
	bgm_->Initialize();
	bgm_->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);
	//�|�X�g�G�t�F�N�g�̏�����
	posteffct_ = make_unique<PostEffect>();
	posteffct_->Initialize();
	cameraeyemove_ = { 0.0f,0.0f,0.0f };
	//�X�|�b�g���C�g���A�N�e�B�u���
	lightgroupe_->SetSpotLightActive(0, true);
	lightgroupe_->SetSpotLightActive(1, true);
	lightgroupe_->SetSpotLightActive(2, true);
}
//�X�e�[�^�X�Z�b�g
void TitleScene::StatusSet()
{
	//�e��UI
	bullet_ui_->Set();
	//�J�����̓���
	titlecamera_->MoveEyeVector(cameraeyemove_);
	titlecamera_->MoveVector(cameramove_);
	//�^�C�g���X�v���C�g�̃X�e�[�^�X�Z�b�g
	title_->SetSize({ titlesize_ });
	title_->SetPosition({ titlepos_ });
	//�}�E�X�J�[�\���̍��W�Z�b�g
	cursor_->SetPosition({ reticlepos_ });
	//1�ڂ̃X�|�b�g���C�g��ݒ�
	lightgroupe_->SetSpotLightDir(0, XMVECTOR({ spotlightdir_.x, spotlightdir_.y, spotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(0, spotlightpos_);
	lightgroupe_->SetSpotLightColor(0, spotlightcolor_);
	lightgroupe_->SetSpotLightAtten(0, spotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(0, spotlightfactorangle_);
	//2�ڂ̃X�|�b�g���C�g��ݒ�
	lightgroupe_->SetSpotLightDir(1, XMVECTOR({ spotlightdir2_.x, spotlightdir2_.y, spotlightdir2_.z }));
	lightgroupe_->SetSpotLightPos(1, spotlightpos2_);
	lightgroupe_->SetSpotLightColor(1, spotlightcolor2_);
	lightgroupe_->SetSpotLightAtten(1, spotlightatten2_);
	lightgroupe_->SetSpotLightFactorAngle(1, spotlightfactorangle2_);
	//1�ڂ̃X�|�b�g���C�g��ݒ�
	lightgroupe_->SetSpotLightDir(2, XMVECTOR({ spotlightdir_.x, spotlightdir_.y, spotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(2, spotlightpos_);
	lightgroupe_->SetSpotLightColor(2, spotlightcolor_);
	lightgroupe_->SetSpotLightAtten(2, spotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(2, spotlightfactorangle_);
	//���̃X�e�[�^�X
	arrowleft_->SetSize(arrowsize_);
	arrowright_->SetSize(arrowsize_);
	arrowleft_->SetColor(arrowleftcolor_);
	arrowright_->SetColor(arrowrightcolor_);
	//�N���b�N�̃X�e�[�^�X
	clickafter_->SetSize(clicksize_);
	clickbefore_->SetSize(clicksize_);
	signalafter_->SetSize(clicksize_);
	signalbefore_->SetSize(clicksize_);
}
//�S�Ă̍X�V�������܂Ƃ߂�
void TitleScene::AllUpdate()
{
	//�|�X�g�G�t�F�N�g�̍X�V����
	posteffct_->Update(postefectcolor_);
	//���C�g�O���[�v�X�V
	lightgroupe_->Update();
	//�w�i�I�u�W�F�N�g�̍X�V
	common_background_->Update();
}
//�X�V����
void TitleScene::Update()
{
	//�}�E�X�̍��W�ɃX�v���C�g�����킹��
	Mouse::GetInstance()->MouseMoveSprite(reticlepos_);
	//UI���C�[�W���O�Ŋg��k�������鏈��
	UiEasingProcess();
	//�J�����̃��[�u�֐�
	CameraDirection();
	//��������̃y�[�W
	DescriptioPageProcess();
	//�Q�[���J�n�̃y�[�W
	GameStartPrepartionPage();
	//�J�[�\�����X�v���C�g�͈͓̔��ł��邩
	Collision::GetInstance()->ToggleFlagInClick(reticlepos_, clickpos_, 250.f, 50.f, signalcurorinflag_);
	//�ŏ��̃N���b�N
	if (signalcurorinflag_) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			titlespriteflag_ = false;
			cameraeyemoveflag_ = true;
		}
	}
	// ������ʂōs���鏈��
	ArrowProces();
	//�t�F�[�h�A�E�g��ɃV�[���`�F���W
	FadeOutAndSceneChange();
	//���W�A��]�A�X�P�[���̃X�e�[�^�X���Z�b�g
	StatusSet();
	//�S�Ă̍X�V����
	AllUpdate();
	//�J�����̍Čv�Z
	titlecamera_->RecalculationMatrix();
}
//�J�����̈ړ�
void TitleScene::CameraDirection()
{
	const float kCameraMoveValueXandY = 0.4f;
	const float kCameraMoveValueZ = 0.1f;
	const float kCameraEyeMoveValue = 0.01f;
	if (titlestate_ != TITLESTATE::kTitleScreen) { return; }
	cameramove_ = { 0.0f,0.0f,0.0f };
	if (cameraeyemoveflag_ && !camerachangeflag_) {
		if (cameraeyemove_.m128_f32[0] >= -1.1) {
			cameraeyemove_.m128_f32[0] -= kCameraEyeMoveValue;
			cameramove_.m128_f32[0] += kCameraMoveValueXandY;
			cameramove_.m128_f32[1] -= kCameraMoveValueXandY;
			cameramove_.m128_f32[2] -= kCameraMoveValueZ;
		}
		else {
			camerachangeflag_ = true;
			cameraeyemove_ = { 0.0f,0.0f,0.0f };
			titlestate_ = TITLESTATE::kDescriptionPage;
		}
	}
}
//UI�̊g��k��
void TitleScene::UiEasingProcess()
{
	const int kTimerLimit = 1;
	const int kAddSize = 5;
	const float kVariableArrowSize = 32;
	const float kAbsoluteValue = 0.05f;
	const XMFLOAT2 kVariableClickSize{ 550,60 };
	if (!easingchangeflag_) {
		easingtimer_ += kAbsoluteValue;
		//�^�C�}�[�����̒l�ɂȂ����甽�]������
		if (easingtimer_ >= kTimerLimit) {
			easingchangeflag_ = true;
		}
	}
	else {
		easingtimer_ -= kAbsoluteValue;
		//�^�C�}�[�����̒l�ɂȂ����甽�]������
		if (easingtimer_ <= -kTimerLimit) {
			easingchangeflag_ = false;
		}
	}
	arrowsize_.x = Action::GetInstance()->EasingOut(easingtimer_, kAddSize) + kVariableArrowSize;
	arrowsize_.y = Action::GetInstance()->EasingOut(easingtimer_, kAddSize) + kVariableArrowSize;
	clicksize_.x = Action::GetInstance()->EasingOut(easingtimer_, kAddSize) + kVariableClickSize.x;
	clicksize_.y = Action::GetInstance()->EasingOut(easingtimer_, kAddSize) + kVariableClickSize.y;
	bullet_ui_->ReloadMotion();
}
// ������ʂōs���鏈��
void TitleScene::ArrowProces()
{
	const XMFLOAT2 kRad{ 16.f,16.f };
	const int kAbsoluteValue = 1;
	//�J�������ړ�������̉��
	if (titlestate_ < TITLESTATE::kGameStartPreoartionPage &&
		camerachangeflag_) {
		if (Collision::GetInstance()->ChangeAtClick(reticlepos_, arrowrightpos_, kRad.x, kRad.y)) {
			arrowrightcolor_ = { 1.f,0.f,0.f,1.f };
			righttrueinflag_ = true;
			//���������ꂽ��
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				titlestate_ += kAbsoluteValue;
			}
		}
		else {
			arrowrightcolor_ = { 1.f,1.f,1.f,1.f };
			righttrueinflag_ = false;
		}
	}
	//�y�[�W��0�ȏ�ł����
	if (titlestate_ > TITLESTATE::kDescriptionPage) {
		if (Collision::GetInstance()->ChangeAtClick(reticlepos_, arrowleftpos_, 16, 16)) {
			arrowleftcolor_ = { 1.f,0.f,0.f,1.f };
			lefttrueinflag_ = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				titlestate_ -= kAbsoluteValue;
				if (titlestate_ <= TITLESTATE::kDescriptionPage) {
					titlestate_ = TITLESTATE::kDescriptionPage;
				}
			}
		}
		else {
			arrowleftcolor_ = { 1.f,1.f,1.f,1.f };
			lefttrueinflag_ = false;
		}
	}

}
//�t�F�[�h�A�E�g��ɃQ�[���V�[���փ`�F���W
void TitleScene::FadeOutAndSceneChange()
{
	const float kSubColor = 0.01f;
	//�~���w����ǂ񂾌�
	if (fadeoutflag_) {
		Action::GetInstance()->ColorDown(postefectcolor_, kSubColor);
		if (postefectcolor_.w <= 0) {
			//�V�[���؂�ւ�
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
}
//�`�揈��
void TitleScene::Draw(DirectXCommon* dxCommon)
{
	posteffct_->PreDrawScene(dxCommon->GetCmdList());
	//�I�u�W�F�N�g�̕`��
	Object3d::PreDraw(dxCommon->GetCmdList());
	common_background_->Draw();
	Object3d::PostDraw();
	//�X�v���C�g�̕`��
	Sprite::PreDraw(dxCommon->GetCmdList());
	if (titlespriteflag_) {
		title_->Draw();
	}
	if (!cameraeyemoveflag_) {
		if (!signalcurorinflag_) {
			clickbefore_->Draw();
		}
		else if (signalcurorinflag_) {
			clickafter_->Draw();
		}
	}
	if (titlestate_ < TITLESTATE::kGameStartPreoartionPage&&titlestate_>TITLESTATE::kTitleScreen) {
		arrowright_->Draw();
	}
	if (titlestate_ > TITLESTATE::kDescriptionPage) {
		arrowleft_->Draw();
	}
	if (titlestate_ == TITLESTATE::kDescriptionPage) {
		descriptionoperation_->Draw();
		bullet_ui_->Draw();
	}
	else if (titlestate_ == TITLESTATE::kEnemyOverviewPage) {
		enemyoverview_->Draw();
	}
	else if (titlestate_ == TITLESTATE::kGameStartPreoartionPage) {
		gamestartpreparation_->Draw();
		if (!signalcurorinflag_) {
			signalbefore_->Draw();
		}
		else if (signalcurorinflag_) {
			signalafter_->Draw();
		}
	}
	cursor_->Draw();
	Sprite::PostDraw();
	posteffct_->PostDrawScene(dxCommon->GetCmdList());
	dxCommon->PreDraw();
	posteffct_->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}
// �����[�h����
void TitleScene::ReloadProcess()
{
	if (Mouse::GetInstance()->PushClick(1)) {
		hudstate_ = UISTATE::kReload;
	}
	if (hudstate_ != UISTATE::kReload) { return; }
	if (remaining_ != 0) {
		//��]�̌��Z����l
		const float kSubRotation = 9.5f;
		//�^�C�}�[�̉��Z����l
		const int kAddTime = 1;
		//�^�C�}�[�����Z���邽�߂̒l
		const int kDivTime = 40;
		int kAnser = 0;
		const int kFull = 0;
		//�c�e����x��\���ɂ���
		remaining_ = MaxRemainingBullet;
		bullet_ui_->SetRemainig(remaining_);
		//�^�C�������Z����
		reloadtime_ += kAddTime;
		//�������Ă���^�C����40�ŏ��Z
		kAnser = reloadtime_ % kDivTime;
		//reloaadtime/40�̗]�肪0�ȊO�̎�
		if (kAnser != 0) { return; }
		//�c�e�}�b�N�X��
		remaining_ = {};
		bullet_ui_->Reload(remaining_);
		//�c�e�����^���ɂȂ�����
		if (remaining_ == kFull) {
			//�^�C����������
			reloadtime_ = {};
			hudstate_ = UISTATE::kWait;
		}
	}
}
// ���C�̏���
void TitleScene::ShotProcess()
{
	const int kSubBullet = 1;
	if (remaining_ < MaxRemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			remaining_ += kSubBullet;
			bullet_ui_->Shot(remaining_);
		}
	}
	
}
// �����̃y�[�W�̏���
void TitleScene::DescriptioPageProcess()
{
	if (titlestate_ != TITLESTATE::kDescriptionPage) { return; }

	ReloadProcess();

	ShotProcess();

	bullet_ui_->FallingUI();
}
// �Q�[���J�n�̃y�[�W�̏���
void TitleScene::GameStartPrepartionPage()
{
	if (titlestate_ != TITLESTATE::kGameStartPreoartionPage) { return; }
	//�~���w�����ĂԂƂ�
	if (camerachangeflag_ &&
		signalcurorinflag_ &&
		titlestate_ == TITLESTATE::kGameStartPreoartionPage) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (clickflag_) {
				clickse_->LoadFile("Resources/Sound/SE/MorseCode.wav", volume_);
				clickflag_ = false;
			}
			fadeoutflag_ = true;
		}
	}
}

//�I������
void TitleScene::Finalize()
{
	title_.reset();
	arrowleft_.reset();
	arrowright_.reset();
	titlecamera_.reset();
	enemyoverview_.reset();
	descriptionoperation_.reset();
	gamestartpreparation_.reset();
	signalafter_.reset();
	signalbefore_.reset();
	cursor_.reset();
	lightgroupe_.reset();
	bgm_.reset();
	clickse_.reset();
	common_background_.reset();
}
