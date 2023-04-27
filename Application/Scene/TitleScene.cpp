#include "TitleScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"Collision.h"
#include"Mouse.h"
#include"HelperMath.h"
#include"SceneManager.h"

const float SubColor = 0.01f;
const float CameraMoveValueXandY = 0.4f;
const float CameraMoveValueZ = 0.1f;
const float CameraEyeMoveValue = 0.01f;
const int MaxRemainingBullet = 9;

const XMFLOAT4 RegularColor = { 1.f,1.f,1.f,1.f };
const XMFLOAT2 DescriptionScreenPosition = { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f };
const XMFLOAT2 SpriteSize = { 72.f,72.f };

//�R���X�g���N�^
TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//����������
void TitleScene::Initialize(DirectXCommon* dxComon)
{
	//�J�����̐���
	titlecamera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(titlecamera_.get());
	//���C�g�̐���
	light_ = make_unique<Light>();
	light_ = Light::Create();
	lightgroupe_ = make_unique<LightGroup>();
	lightgroupe_ = LightGroup::Create();
	//���C�g�̃Z�b�g
	Object3d::SetLightGroup(lightgroupe_.get());
	Object3d::SetLight(light_.get());
	//�X�v���C�g�̐���
	title_.reset(Sprite::SpriteCreate(Name::kTitle, { 1.0f,1.0f }));
	cursor_.reset(Sprite::SpriteCreate(Name::kReticle, reticlepos_, spritecol_, anchorpoint_));
	clickbefore_.reset(Sprite::SpriteCreate(Name::kClickStartBefore, clickpos_, RegularColor, anchorpoint_));
	clickafter_.reset(Sprite::SpriteCreate(Name::kClickStartAfter, clickpos_, RegularColor, anchorpoint_));
	signalbefore_.reset(Sprite::SpriteCreate(Name::kSignalBefore, clickpos_, RegularColor, anchorpoint_));
	signalafter_.reset(Sprite::SpriteCreate(Name::kSignalAfter, clickpos_, RegularColor, anchorpoint_));
	descriptionoperation_.reset(Sprite::SpriteCreate(Name::kOperationDescription, DescriptionScreenPosition, spritecol_, anchorpoint_));
	enemyoverview_.reset(Sprite::SpriteCreate(Name::kEnemyDescription, DescriptionScreenPosition, spritecol_, anchorpoint_));
	gamestartpreparation_.reset(Sprite::SpriteCreate(Name::kStartScreen, DescriptionScreenPosition, spritecol_, anchorpoint_));
	arrowright_.reset(Sprite::SpriteCreate(kArrowRight, arrowrightpos_, arrowrightcolor_, anchorpoint_));
	arrowleft_.reset(Sprite::SpriteCreate(kArrowLeft, arrowleftpos_, arrowleftcolor_, anchorpoint_));
	
	bullet_ui_ = make_unique<BulletUI>();
	bullet_ui_->Create(remaining_,ui_bulletpos_,ui_reloadpos_);

	//�I�u�W�F�N�g�̐���
	sphere_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSkydome));
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
		builslowalpha_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
	}
	start_ = Object3d::Create(ModelManager::GetInstance()->GetModel(8));
	world_ = Object3d::Create(ModelManager::GetInstance()->GetModel(9));
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
	bullet_ui_->Set();

	//�J�����̓���
	titlecamera_->MoveEyeVector(cameraeyemove_);
	titlecamera_->MoveVector(cameramove_);
	//�V���̃X�e�[�^�X�̃Z�b�g
	sphere_->SetRotation(sphererot_);
	sphere_->SetPosition(spherepos_);
	sphere_->SetScale(spherescl_);
	//�n�ʂ̃X�e�[�^�X�̃Z�b�g
	world_->SetPosition(worldpos_);
	world_->SetScale(worldscl_);
	//���E�̃r���̃X�e�[�^�X�̃Z�b�g
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i]->SetScale(builsscl_);
		builslowalpha_[i]->SetScale(builsscl_);
		if (i % 2 == 0) {
			builshighalphapos_ = { 100.0f, 0,-300.0f + (100 * i / 2) };
			builslowalphapos_ = { 200.0f,0,-300.0f + (100 * i / 2) };
			builsrot_ = { 0.0f,90.0f,0.0f };
		}
		else if (i % 2 == 1) {
			builshighalphapos_ = { -100.0f,0,-300.0f + (100 * i / 2) };
			builslowalphapos_ = { -200.0f, 0,-300.0f + (100 * i / 2) };
			builsrot_ = { 0.0f,270.0f,0.0f };
		}
		builshighalpha_[i]->SetRotation(builsrot_);
		builshighalpha_[i]->SetPosition(builshighalphapos_);
		builslowalpha_[i]->SetRotation(builsrot_);
		builslowalpha_[i]->SetPosition(builslowalphapos_);
	}
	//�J�����̈ړ���̃r���̃X�e�[�^�X�Z�b�g
	start_->SetPosition(startpos_);
	start_->SetScale(startscl_);
	start_->SetRotation({ 0.0f,180.0f,0.0f });
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

	arrowleft_->SetSize(arrowsize_);
	arrowright_->SetSize(arrowsize_);
	arrowleft_->SetColor(arrowleftcolor_);
	arrowright_->SetColor(arrowrightcolor_);

	clickafter_->SetSize(clicksize_);
	clickbefore_->SetSize(clicksize_);
	signalafter_->SetSize(clicksize_);
	signalbefore_->SetSize(clicksize_);
}

//�S�Ă̍X�V�������܂Ƃ߂�
void TitleScene::AllUpdate()
{
	//���E�̃r���̍X�V����
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i]->Update({ 0.8f,0.6f,0.3f,1.0f });
		builslowalpha_[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}
	//�|�X�g�G�t�F�N�g�̍X�V����
	posteffct_->Update(postefectcolor_);
	//�V���̍X�V����
	sphere_->Update({ 1,1,1,1 }, true);
	//�n�ʂ̍X�V����
	world_->Update();
	//�J�����̈ړ���̃r���̍X�V����
	start_->Update({ 0.4f,0.4f,0.4f,1.f });
	//���C�g�O���[�v�X�V
	lightgroupe_->Update();
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
	if (titlestate_ != TITLESCREEN) { return; }
	cameramove_ = { 0.0f,0.0f,0.0f };
	if (cameraeyemoveflag_ && !camerachangeflag_) {
		if (cameraeyemove_.m128_f32[0] >= -1.1) {
			cameraeyemove_.m128_f32[0] -= CameraEyeMoveValue;
			cameramove_.m128_f32[0] += CameraMoveValueXandY;
			cameramove_.m128_f32[1] -= CameraMoveValueXandY;
			cameramove_.m128_f32[2] -= CameraMoveValueZ;
		}
		else {
			camerachangeflag_ = true;
			cameraeyemove_ = { 0.0f,0.0f,0.0f };
			titlestate_ = DESCRIPTIONPAGE;
		}
	}
}
//UI�̊g��k��
void TitleScene::UiEasingProcess()
{
	if (!easingchangeflag_) {
		easingtimer_ += 0.05f;
		//�^�C�}�[�����̒l�ɂȂ����甽�]������
		if (easingtimer_ >= 1) {
			easingchangeflag_ = true;
		}
	}
	else {
		easingtimer_ -= 0.05f;
		//�^�C�}�[�����̒l�ɂȂ����甽�]������
		if (easingtimer_ <= -1) {
			easingchangeflag_ = false;
		}
	}
	arrowsize_.x = Action::GetInstance()->EasingOut(easingtimer_, 5) + 32;
	arrowsize_.y = Action::GetInstance()->EasingOut(easingtimer_, 5) + 32;
	clicksize_.x = Action::GetInstance()->EasingOut(easingtimer_, 5) + 550;
	clicksize_.y = Action::GetInstance()->EasingOut(easingtimer_, 5) + 60;
	bullet_ui_->ReloadMotion();
}

void TitleScene::ArrowProces()
{
	//�J�������ړ�������̉��
	if (titlestate_ < GAMESTARTPREPARTIONPAGE &&
		camerachangeflag_) {
		if (Collision::GetInstance()->ChangeAtClick(reticlepos_, arrowrightpos_, 16, 16)) {
			arrowrightcolor_ = { 1.f,0.f,0.f,1.f };
			righttrueinflag_ = true;
			//���������ꂽ��
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				titlestate_ += 1;
			}
		}
		else {
			arrowrightcolor_ = { 1.f,1.f,1.f,1.f };
			righttrueinflag_ = false;
		}
	}
	//�y�[�W��0�ȏ�ł����
	if (titlestate_ > DESCRIPTIONPAGE) {
		if (Collision::GetInstance()->ChangeAtClick(reticlepos_, arrowleftpos_, 16, 16)) {
			arrowleftcolor_ = { 1.f,0.f,0.f,1.f };
			lefttrueinflag_ = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				titlestate_ -= 1;
				if (titlestate_ <= DESCRIPTIONPAGE) {
					titlestate_ = DESCRIPTIONPAGE;
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
	//�~���w����ǂ񂾌�
	if (fadeoutflag_) {
		postefectcolor_.x -= SubColor;
		postefectcolor_.y -= SubColor;
		postefectcolor_.z -= SubColor;
		postefectcolor_.w -= SubColor;
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
	Object3d::PreDraw(dxCommon->GetCmdList());
	world_->Draw();
	sphere_->Draw();
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i]->Draw();
		builslowalpha_[i]->Draw();
	}
	start_->Draw();
	Object3d::PostDraw();
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

	if (titlestate_ < GAMESTARTPREPARTIONPAGE) {
		arrowright_->Draw();
	}
	if (titlestate_ > DESCRIPTIONPAGE) {
		arrowleft_->Draw();
	}
	if (titlestate_ == DESCRIPTIONPAGE) {
		descriptionoperation_->Draw();
		bullet_ui_->Draw();
	}
	else if (titlestate_ == ENEMYOVERVIEWPAGE) {
		enemyoverview_->Draw();
	}
	else if (titlestate_ == GAMESTARTPREPARTIONPAGE) {
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

void TitleScene::ReloadProcess()
{
	if (Mouse::GetInstance()->PushClick(1)) {
		hudstate_ = RELOAD;
	}

	if (hudstate_ != RELOAD) { return; }
	if (remaining_ != 0) {
		//��]�̌��Z����l
		const float subrotation_ = 9.5f;
		//�^�C�}�[�̉��Z����l
		const int addtime_ = 1;
		//�^�C�}�[�����Z���邽�߂̒l
		const int divtime_ = 40;
		int anser_ = 0;
		//�c�e����x��\���ɂ���
		remaining_ = 8;
		bullet_ui_->SetRemainig(remaining_);
		//�^�C�������Z����
		reloadtime_ += addtime_;
		//�������Ă���^�C����40�ŏ��Z
		anser_ = reloadtime_ % divtime_;
		//reloaadtime/40�̗]�肪0�ȊO�̎�
		if (anser_ != 0) { return; }
		//�c�e�}�b�N�X��
		remaining_ = {};
		bullet_ui_->Reload(remaining_);
		//�c�e�����^���ɂȂ�����
		if (remaining_ == 0) {
			//�^�C����������
			reloadtime_ = {};
			hudstate_ = WAIT;
		}
	}
}

void TitleScene::ShotProcess()
{
	if (remaining_ < MaxRemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			remaining_ += 1;
			bullet_ui_->Shot(remaining_);
		}
	}
	
}

void TitleScene::DescriptioPageProcess()
{
	if (titlestate_ != DESCRIPTIONPAGE) { return; }

	ReloadProcess();

	ShotProcess();

	bullet_ui_->FallingUI();
}

void TitleScene::GameStartPrepartionPage()
{
	if (titlestate_ != GAMESTARTPREPARTIONPAGE) { return; }
	//�~���w�����ĂԂƂ�
	if (camerachangeflag_ &&
		signalcurorinflag_ &&
		titlestate_ == GAMESTARTPREPARTIONPAGE) {
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
	light_.reset();
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i].reset();
		builslowalpha_[i].reset();
	}
	sphere_.reset();
	world_.reset();
	start_.reset();
	bgm_.reset();
	clickse_.reset();
}
