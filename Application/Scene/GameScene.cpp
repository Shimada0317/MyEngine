#include "GameScene.h"
#include<cassert>
#include <iomanip>
#include"Action.h"
#include"Mouse.h"
#include"HelperMath.h"

#include"SceneManager.h"

const float AddPosetEfectColor = 0.05f;
const XMFLOAT4 BillColor = { 0.8f,0.6f,0.3f,1.0f };

using namespace DirectX;


//�R���X�g���N�^
GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//����������
void GameScene::Initialize(DirectXCommon* dxComon)
{
	//�|�X�g�G�t�F�N�g�̐���
	posteffect_ = make_unique<PostEffect>();
	posteffect_->Initialize();
	posteffect_->Update(postcol_);

	//���C�g�̐���
	light_ = Light::Create();
	lightgroupe_ = LightGroup::Create();
	//���C�g�Z�b�g
	Object3d::SetLight(light_.get());
	Object3d::SetLightGroup(lightgroupe_.get());
	//�J�����̐���
	camera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);


	//�X�v���C�g�̐���
	damageefectsprite_.reset(Sprite::SpriteCreate(Name::kDamageEffect, { 0.0f, 0.0f }, damageefectcolor_));
	clear_.reset(Sprite::SpriteCreate(kGameClear, { 0.0f,0.0f }));
	conteniu_.reset(Sprite::SpriteCreate(kGameOver, { 0.0f,0.0f }));
	shot_.reset(Sprite::SpriteCreate(kShot, { 0.f,WinApp::window_height - 150 }));
	reticleforgameover_.reset(Sprite::SpriteCreate(kReticle, ReticlePosition, ReticleColor, SpriteAnchorPoint));
	yes_.reset(Sprite::SpriteCreate(kYes, YesPosition, YesColor, SpriteAnchorPoint));
	no_.reset(Sprite::SpriteCreate(kNo, NoPosition, NoColor, SpriteAnchorPoint));
	hart_.reset(Sprite::SpriteCreate(kHart, HartPosition, { 1.f,1.f,1.f,1.f }, SpriteAnchorPoint));
	curtainup_.reset(Sprite::SpriteCreate(Name::kCurtain, CurtainUpPos));
	curtaindown_.reset(Sprite::SpriteCreate(Name::kCurtain, CurtainDownPos));
	skip_.reset(Sprite::SpriteCreate(Name::kSkip, SkipPos));
	for (int i = 0; i < 5; i++) {
		lifecount_[i].reset(Sprite::SpriteCreate(i, HartPosition));
	}

	curtainup_->SetSize(CurtainSize);
	curtaindown_->SetSize(CurtainSize);

	//���f���̓ǂݍ���
	sphere_ = Object3d::Create(ModelManager::GetInstance()->GetModel(6));
	for (int i = 0; i < BUILS; i++) {
		builshighalpha_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
		builslowalpha_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));

	}
	for (int i = 0; i < 5; i++) {
		fieldbuils_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
	}
	world_ = Object3d::Create(ModelManager::GetInstance()->GetModel(9));
	start_ = Object3d::Create(ModelManager::GetInstance()->GetModel(8));

	heri_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	goal_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	hane_ = Object3d::Create(ModelManager::GetInstance()->GetModel(12));

	player_ = make_unique<Player>();
	player_->Initalize(camera_.get());
	playerhp_ = player_->GetHp();

	railcamera_ = make_unique<RailCamera>();
	railcamera_->MatrixIdentity(player_->GetPosition(), player_->GetRotation());
	railcamera_->Update(velocity_, eyerot_, camera_.get());


	oldhp_ = playerhp_;

	bgm_ = make_unique<Audio>();
	bgm_->Initialize();
	bgm_->LoopWave("Resources/Sound/BGM/Blinded.wav", 0.3f);

	herifry_ = make_unique<Audio>();
	herifry_->Initialize();
	herifry_->LoadFile("Resources/Sound/SE/heriFry.wav", 0.8f);

	fieldbillpos_[0] = { -35.0f,0.0f,20.0f };
	fieldbillpos_[1] = { -7.0f,-1,80 };
	fieldbillpos_[2] = { 37.0,0.0f,-10.0f };
	fieldbillpos_[3] = { -900.0,0.0f,0.0f };;
	fieldbillpos_[4] = { -900.0,0.0f,0.0f };;

	fieldbillrot_[0] = { 0.0f,270.0f,0.0f };
	fieldbillrot_[1] = { 0.0f,0.0f,0.0f };
	fieldbillrot_[2] = { 0.0f,90.0f,0.0f };
	fieldbillrot_[3] = { 0.0f,0.0f,0.0f };
	fieldbillrot_[4] = { 0.0f,0.0f,0.0f };


	for (int i = 0; i < 3; i++) {
		searchlightdir_[i] = { 0,-10,0 };
		searchlightcolor_[i] = { 1.f,1.f,1.f };
	}
	searchlightpos_[0] = { 0, 20, 20 };
	searchlightpos_[1] = { 20, 10, 45 };
	searchlightpos_[2] = { 54,10,43 };
	for (int i = 0; i < 2; i++) {
		LightPositionChangeX[i] = false;
		LightPositionChangeZ[i] = false;
	}

	lightgroupe_->SetSpotLightActive(0, true);
	lightgroupe_->SetSpotLightActive(1, true);
	lightgroupe_->SetSpotLightActive(2, true);
	lightgroupe_->SetSpotLightActive(3, true);
	lightgroupe_->SetSpotLightActive(4, true);
	MotValue = HartSize;
	LoadEnemyPopData();
}

//�X�e�[�^�X�Z�b�g
void GameScene::StatusSet()
{



#pragma region ��ŕK�v�ȕϐ��̒ǉ������̃R�����g������
	//�ϓ�����J�E���^�[
	for (int i = 0; i < 5; i++) {
		lifecount_[i]->SetSize({ 80,80 });
		lifecount_[i]->SetPosition({ 1160,630 });
	}

	hart_->SetSize(HartSize);
	hart_->SetPosition({ WinApp::window_width - 173,WinApp::window_height - 50 });
	//Hp�o�[
	player_->SetHp(playerhp_);

	heri_->SetPosition(heripos_);
	heri_->SetScale(heriscl_);
	heri_->SetRotation({ 0.0f,180.0f,0.0f });

	hane_->SetRotation({ 0.0f,HeriY,0.0f });
	if (StartMovieFlag == false) {
		hane_->SetPosition(heripos_);
		hane_->SetScale(heriscl_);
	}
	else {
		hane_->SetPosition(goalpos_);
		hane_->SetScale(goalscl_);
	}

	goal_->SetPosition(goalpos_);
	goal_->SetScale(goalscl_);
	goal_->SetRotation({ 0.0f,270.0f,0.0f });

	heri_->Update({ 0.7f,0.7f,0.6f,1.0f });
	goal_->Update({ 0.7f,0.7f,0.6f,1.0f });
	hane_->Update({ 0.0f,0.0f,0.0f,1.0f });

#pragma endregion


	//�V���̃X�e�[�^�X�Z�b�g
	sphere_->SetRotation(sphererot_);
	sphere_->SetPosition(spherepos_);
	sphere_->SetScale(spherescl_);

	//�n�ʂ̃X�e�[�^�X�Z�b�g
	world_->SetPosition(worldpos_);
	world_->SetScale(worldscl_);

	//�A�N�^�[�N���X����Q�b�g
	playerhp_ = player_->GetHp();

	//�X�^�[�g�n�_�̃X�e�[�^�X�Z�b�g
	start_->SetPosition(startpos_);
	start_->SetScale(startscl_);
	start_->SetRotation(startrot_);

	//���E�̃r���̃X�e�[�^�X�Z�b�g
	for (int i = 0; i < BUILS; i++) {
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

	//�t�B�[���h�̌����̃X�e�[�^�X�Z�b�g
	for (int i = 0; i < 5; i++) {
		fieldbuils_[i]->SetPosition(fieldbillpos_[i]);
		fieldbuils_[i]->SetRotation(fieldbillrot_[i]);
		fieldbuils_[i]->SetScale(fieldbuilscl_);
	}

	damageefectsprite_->SetColor(damageefectcolor_);


	lightgroupe_->SetSpotLightDir(0, XMVECTOR({ fieldspotlightdir_.x, fieldspotlightdir_.y, fieldspotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(0, fieldspotlightpos_);
	lightgroupe_->SetSpotLightColor(0, fieldspotlightcolor_);
	lightgroupe_->SetSpotLightAtten(0, fieldspotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(0, fieldspotlightfactorangle_);

	lightgroupe_->SetSpotLightDir(1, XMVECTOR({ playerspotlightdir_.x, playerspotlightdir_.y, playerspotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(1, playerspotlightpos_);
	lightgroupe_->SetSpotLightColor(1, playerspotlightcolor_);
	lightgroupe_->SetSpotLightAtten(1, playerspotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(1, playerspotlightfactorangle_);

	for (int i = 2; i < 5; i++) {
		lightgroupe_->SetSpotLightDir(i, XMVECTOR({ searchlightdir_[i - 2].x, searchlightdir_[i - 2].y, searchlightdir_[i - 2].z }));
		lightgroupe_->SetSpotLightPos(i, searchlightpos_[i - 2]);
		lightgroupe_->SetSpotLightColor(i, searchlightcolor_[i - 2]);
		lightgroupe_->SetSpotLightAtten(i, searchlightatten_);
		lightgroupe_->SetSpotLightFactorAngle(i, searchlightfactorangle_);
	}

};

//�I�u�W�F�N�g�Ȃǂ̍X�V����
void GameScene::AllUpdata()
{
	Action::GetInstance()->DebugMove(searchlightpos_[0]);
	if (GetCamWorkFlag == true) {
		velocity_ = XMVector3TransformNormal(velocity_, player_->GetBodyMatrix());
	}
	railcamera_->Update(velocity_, eyerot_, camera_.get());
	//���E�̃r���̍X�V����
	for (int i = 0; i < BUILS; i++) {
		builshighalpha_[i]->Update(BillColor);
		builslowalpha_[i]->Update(BillColor);
	}
	//�t�B�[���h�̃r���̍X�V����
	for (int i = 0; i < 5; i++) {
		fieldbuils_[i]->Update(BillColor);
	}
	//�V���̍X�V����
	sphere_->Update({ 1,1,1,1 }, true);
	//�n�ʂ̍X�V����
	world_->Update({ 0.7f,0.7f,0.7f,1.0f });
	//�X�^�[�g�n�_�̍X�V����
	start_->Update(BillColor);

	player_->Update(camera_.get(), (Phase)patern_,pathrot_);
}

//�Q�[���V�[���̍X�V����
void GameScene::Update()
{
	PlayerMove();

	StartCameraWork();

	SpotLightMove();

	if (gamestartflag_ == false) {
		FadeIn();
	}
	if (gamestartflag_ == true) {
		DamageProcess();
	}

	//�S�[���ɒ������Ƃ�
	if (goalpos_.m128_f32[1] >= 100) {
		clearflag_ = true;
		stopupdateflag_ = true;
	}
	//�S�[���ɂ��Ă��Ȃ��Ƃ��X�V�𑱂���
	if (stopupdateflag_ == false) {
		StatusSet();
		AllUpdata();
	}
	else if (stopupdateflag_ == true && clearflag_ == false)
	{
		GameOverProcess();
	}

	GameClearProcesss();

	if (screenshakestate_ != NONE) {
		ScreenShake(shakingscreenvalue_);
	}

#pragma region Actor����Update���̏����������Ă���(��ł��̃R�����g�͏���)

	EasingTimer += AddTimer;
	if (ReversFlag == true) {
		HartSize.x = Action::GetInstance()->EasingOut(EasingTimer, 40) + MotValue.x;
		HartSize.y = Action::GetInstance()->EasingOut(EasingTimer, 40) + MotValue.y;
		if (EasingTimer >= 1) {
			EasingTimer = 0;
			MotValue = HartSize;
			ReversFlag = false;
		}
	}
	else {
		HartSize.x = -Action::GetInstance()->EasingOut(EasingTimer, 40) + MotValue.x;
		HartSize.y = -Action::GetInstance()->EasingOut(EasingTimer, 40) + MotValue.y;
		if (EasingTimer >= 1) {
			EasingTimer = 0;
			MotValue = HartSize;
			ReversFlag = true;
		}
	}


	if (playerhp_ == 4) {
		AddTimer = 0.01f;
	}
	else if (playerhp_ == 3) {
		AddTimer = 0.05f;
	}
	else if (playerhp_ == 2) {
		AddTimer = 0.1f;
	}
	else if (playerhp_ == 1) {
		AddTimer = 0.5f;
	}


	XMVECTOR velo = player_->GetVelocity();
	//SetVelocity(velo);

	heripos_.m128_f32[2] += HeriX;

	if (heripos_.m128_f32[2] >= 20) {
		BackObjFlag = false;
		StartMovieFlag = true;
	}
	else {
		HeriY += 15.0f;
	}

	if (GetCamWorkFlag == true) {
		robot_.remove_if([](std::unique_ptr<Enemy>& robot) {
			return robot->IsDead();
			});
		boss_.remove_if([](std::unique_ptr<BossEnemy>& boss) {
			return boss->IsDead();
			});
		//�ڂ̑O�̓G��S�ē|�������v���C���[�𓮂���
		if (robot_.empty() && boss_.empty()) {
			MoveFlag = true;
		}
		//�v���C���[���ړI�n�_�ɒ������Ƃ�
		if (StopFlag == true) {
			MoveFlag = false;
			UpdataEnemyPopCommands();
			patern_ += 1;
			StopFlag = false;
		}
	}

	if (patern_ >= 5) {
		HeriY += 15.0f;
	}

	if (patern_ >= 6) {
		if (fring == true) {
			goalpos_.m128_f32[1] += velo.m128_f32[1];
		}
	}

	XMFLOAT2 Player2DPos = player_->GetRetPosition();
	bool PlayerBulletShot_F = player_->GetBulletShot();
	//�G�̍X�V����
	for (std::unique_ptr<Enemy>& Enemy : robot_) {
		Enemy->Update(Player2DPos, playerhp_, PlayerBulletShot_F);
	}

	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Update(Player2DPos, playerhp_, PlayerBulletShot_F);
	}

	CheckSameTrackPosition();
	player_->SetBulletShot(PlayerBulletShot_F);

	camera_->RecalculationMatrix();
#pragma endregion

	posteffect_->Update(postcol_);
	lightgroupe_->Update();
}

//�I�u�W�F�N�g�̕`�揈��
void GameScene::ObjDraw(DirectXCommon* dxCommon)
{
	////�I�u�W�F�N�g�O����
	Object3d::PreDraw(dxCommon->GetCmdList());
	sphere_->Draw();
	world_->Draw();
	start_->Draw();
	for (int i = 0; i < 5; i++) {
		fieldbuils_[i]->Draw();
	}
	for (int i = 0; i < BUILS; i++) {
		builshighalpha_[i]->Draw();
		builslowalpha_[i]->Draw();
	}

#pragma region Actor����Draw�̏����������Ă���(��ŏ���)
	goal_->Draw();
	hane_->Draw();
	if (BackObjFlag == true) {
		heri_->Draw();
	}
	if (GameState != MOVIE) {
		player_->ObjDraw();
	}

	player_->ParticleDraw(dxCommon->GetCmdList());
#pragma endregion


	////�I�u�W�F�N�g�㏈��
	Object3d::PostDraw();
	for (std::unique_ptr<Enemy>& robot : robot_) {
		robot->Draw(dxCommon);
	}
	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Draw(dxCommon);
	}
}

//�X�v���C�g�̕`�揈��
void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	shot_->Draw();

	if (damagehitflag_ == true) {
		damageefectsprite_->Draw();
	}
	if (clearflag_ == true) {
		clear_->Draw();
	}
	if (dethflag_ == true) {
		conteniu_->Draw();
		no_->Draw();
		yes_->Draw();
		reticleforgameover_->Draw();
	}

	if (GetCamWorkFlag == true) {
		if (playerhp_ == 1) {
			lifecount_[0]->Draw();
		}
		else if (playerhp_ == 2) {
			lifecount_[1]->Draw();
		}
		else if (playerhp_ == 3) {
			lifecount_[2]->Draw();
		}
		else if (playerhp_ == 4) {
			lifecount_[3]->Draw();
		}
		else if (playerhp_ == 5) {
			lifecount_[4]->Draw();
		}
		hart_->Draw();
	}

	if (GetCamWorkFlag == false && startflag_ == false) {
		curtainup_->Draw();
		curtaindown_->Draw();
		skip_->Draw();

	}
	if (GameState != MOVIE) {
		player_->SpriteDraw();
	}
	Sprite::PostDraw();
}

//ImgUi�̕`�揈��
void GameScene::ImgDraw()
{

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Camera");
	/*ImGui::SliderFloat("LightPosX", &searchlightpos_[0].x, -100.0f, 100.0f);
	ImGui::SliderFloat("LightPosY", &searchlightpos_[0].y, -100.0f, 100.0f);
	ImGui::SliderFloat("LightPosZ", &searchlightpos_[0].z, -100.0f, 100.0f);*/

	ImGui::SliderInt("Actioncount", &actioncount_, -100, 100);
	ImGui::SliderFloat("Actiontimer", &actiontimer_, -100.0f, 100.0f);
	ImGui::SliderFloat("eyerot", &pathrot_.y, -180.0f, 180.0f);

	

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

//�|�X�g�G�t�F�N�g�̕`�揈��
void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{
	posteffect_->PreDrawScene(dxCommon->GetCmdList());
	ObjDraw(dxCommon);

	posteffect_->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	posteffect_->Draw(dxCommon->GetCmdList());
	SpriteDraw(dxCommon);
	//�`��㏈��
	ImgDraw();
	player_->ImGuiDraw();
	dxCommon->PostDraw();
}

//�`�揈��
void GameScene::Draw(DirectXCommon* dxCommon)
{
	PostEffectDraw(dxCommon);
}

//�I������
void GameScene::Finalize()
{
	conteniu_.reset();
	clear_.reset();
	shot_.reset();


	world_.reset();
	start_.reset();
	for (int i = 0; i < BUILS; i++) {
		builshighalpha_[i].reset();
		builslowalpha_[i].reset();
	}
	for (int i = 0; i < 5; i++) {
		fieldbuils_[i].reset();
	}
}

void GameScene::FadeIn()
{
	postcol_.x += AddPosetEfectColor;
	postcol_.y += AddPosetEfectColor;
	postcol_.z += AddPosetEfectColor;
	if (postcol_.x >= 0.0f) {
		postcol_.x = 0.0f;
		postcol_.y = 0.0f;
		postcol_.z = 0.0f;
		gamestartflag_ = true;
	}
}

void GameScene::SpotLightMove()
{
	if (Easing == false) {
		EasingWaitTimeR += 0.1f;
		if (EasingWaitTimeR >= 1) {
			Easing = true;
			EasingWaitTimeR = 0.f;
		}
	}

	if (Easing == true) {
		if (EasingChange == false) {
			duration = 1;
			if (duration > time) {
				time += 0.01f;
			}
		}
		else {
			duration = -1;
			if (duration < time) {
				time -= 0.01f;
			}
		}
	}
	if (ChangeTimerFlag == false) {
		LightAddPosChangeTimer += 0.01f;
	}
	else {
		LightAddPosChangeTimer -= 0.01f;
	}
	if (LightAddPosChangeTimer >= 1) {
		SearchLightAddPos = HelperMath::GetInstance()->XMFLOAT3ChangeValue(SearchLightAddPos);
		ChangeTimerFlag = true;
	}
	else if (LightAddPosChangeTimer <= -1) {
		SearchLightAddPos = HelperMath::GetInstance()->XMFLOAT3ChangeValue(SearchLightAddPos);
		ChangeTimerFlag = false;
	}

	if (LightDirEasingChane == false) {
		LightDirEasingTime += 0.05f;
	}
	else {
		LightDirEasingTime -= 0.05f;
	}

	searchlightdir_[0].x = Action::GetInstance()->EasingOut(LightDirEasingTime, 5 - 0);
	searchlightdir_[1].z = Action::GetInstance()->EasingOut(LightDirEasingTime, 5 - 0);
	searchlightdir_[2].x = Action::GetInstance()->EasingOut(LightDirEasingTime, 5 - 0);

	searchlightdir_[0].z = Action::GetInstance()->EasingOut(time, EndPointZ - StartPointZ);
	searchlightdir_[1].x = Action::GetInstance()->EasingOut(time, EndPointX - StartPointX);
	searchlightdir_[2].z = Action::GetInstance()->EasingOut(time, EbdPointZ2 - StartPointZ2);



	if (patern_ == 8 && MoveFlag == true) {
		if (ColorTime >= 0) {
			ColorTime -= 0.01f;
		}
		if (ColorTimeRed <= 1) {
			ColorTimeRed += 0.01f;
		}

		fieldspotlightcolor_.x = Action::GetInstance()->EasingOut(ColorTimeRed, EndColorRed - StartColorRed) + 0.9f;
		fieldspotlightcolor_.y = Action::GetInstance()->EasingOut(ColorTime, EndColor - StartColor);
	}

	if (SpotLightPositionChange == false) {
		if (time >= 1.f) {
			SpotLightPositionChange = true;
			Easing = false;
			EasingChange = true;
		}
	}
	else {
		if (time <= -1.f) {
			SpotLightPositionChange = false;
			Easing = false;
			EasingChange = false;
		}
	}
}

void GameScene::DamageProcess()
{
	if (playerhp_ > 0) {
		//�_���[�W��H��������Ƃ�
		if (oldhp_ > playerhp_) {
			posteffectonflag_ = true;
			damagehitflag_ = true;
			damageefectcolor_.w = 1;
			oldhp_ = playerhp_;
			screenshakestate_ = DAMAGE;
			shakingscreenvalue_ = 5.f;
		}
		//��ʂ�Ԃ�����t���O����������
		if (posteffectonflag_ == true) {
			postcol_.x = 0.7f;
			if (postcol_.x >= 0.7f) {
				posteffectonflag_ = false;
			}
		}
		//��ʂ�Ԃ�����t���O�������Ă��Ȃ���
		if (posteffectonflag_ == false) {
			postcol_.x -= 0.05f;
			if (postcol_.x <= 0) {
				postcol_.x = 0;
			}
		}
	}
	//�̗͂�0�ɂȂ�����
	else if (playerhp_ <= 0) {
		stopupdateflag_ = true;
		postcol_.x += 0.01f;
		if (postcol_.x >= 2.0f) {
			dethflag_ = true;
		}
	}

	if (damagehitflag_ == true) {
		damageefectcolor_.w -= 0.02f;
		if (damageefectcolor_.w <= 0) {
			damagehitflag_ = false;
		}
	}
}

void GameScene::GameOverProcess()
{
	Mouse::GetInstance()->MouseMoveSprite(ReticlePosition);
	reticleforgameover_->SetPosition(ReticlePosition);
	if (dethflag_ == true) {
		postcol_.x = 0;
		CheckcCursorIn(ReticlePosition, YesPosition, 100, 50, YesCursorInFlag);
		CheckcCursorIn(ReticlePosition, NoPosition, 100, 50, NoCursorInFlag);

		if (YesCursorInFlag == true) {
			YesColor = { 1.f,0.f,0.f,1.f };
			if (Mouse::GetInstance()->PushClick(0)) {
				BaseScene* scene_ = new GameScene(sceneManager_);
				sceneManager_->SetNextScene(scene_);
			}
		}
		else {
			YesColor = { 1.f,1.f,1.f,1.f };
		}

		if (NoCursorInFlag == true) {
			NoColor = { 1.f,0.f,0.f,1.f };
			if (Mouse::GetInstance()->PushClick(0)) {
				BaseScene* scene_ = new TitleScene(sceneManager_);
				sceneManager_->SetNextScene(scene_);
			}
		}
		else {
			NoColor = { 1.f,1.f,1.f,1.f };
		}
		yes_->SetColor(YesColor);
		no_->SetColor(NoColor);
	}
}

void GameScene::GameClearProcesss()
{
	//�S�[���ɒ�������N���A��ʂ�\��
	if (clearflag_ == true) {
		if (Mouse::GetInstance()->PushClick(0)) {
			BaseScene* scene_ = new TitleScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
}

void GameScene::LoadEnemyPopData()
{
	ifstream file;
	file.open("Resources/LoadEnemy.csv");
	assert(file.is_open());

	enemypopcommands_ << file.rdbuf();

	file.close();
}

void GameScene::UpdataEnemyPopCommands()
{
	//�ҋ@����
	if (WaitFlag == true) {
		WaitT--;
		if (WaitT <= 0) {
			WaitFlag = false;
		}
		return;
	}

	std::string line;

	XMVECTOR POSITION = { 0.0f,0.0f,0.0f };
	XMVECTOR TRACK = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ROTATION = { 0.0f,0.0f,0.0f };
	bool ari = false;
	bool step = false;
	int count = 0;

	bool POPSkip = false;
	bool TRACKSkip = false;
	bool ARIVESkip = false;

	while (getline(enemypopcommands_, line))
	{

		std::istringstream line_stram(line);

		std::string word;

		getline(line_stram, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("WAVE") == 0) {
			getline(line_stram, word, ',');
			//WAVE�̗v�f
			count = atoi(word.c_str());
		}
		if (patern_ == count) {
			if (word.find("ROTATION") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				ROTATION.x = x;
				ROTATION.y = y;
				ROTATION.z = z;

				POPSkip = true;
			}
			//���W�擾
			else if (word.find("POSITION") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				POSITION.m128_f32[0] = x;
				POSITION.m128_f32[1] = y;
				POSITION.m128_f32[2] = z;

				POPSkip = true;
			}
			//�ǔ���̍��W�擾
			else if (word.find("TRACK") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				TRACK.m128_f32[0] = x;
				TRACK.m128_f32[1] = y;
				TRACK.m128_f32[2] = z;

				TRACKSkip = true;
			}

			else if (word.find("STEP") == 0) {
				getline(line_stram, word, ',');
				int STEP = atoi(word.c_str());
				if (STEP == 1) {
					step = true;
				}
				else {
					step = false;
				}

			}

			//
			else if (word.find("ARIVE") == 0) {
				getline(line_stram, word, ',');

				//�҂�����
				int Arive = atoi(word.c_str());
				if (Arive == 1) {
					ari = true;
				}
				else {
					ari = false;
				}

				ARIVESkip = true;
			}

			if (ARIVESkip == true && POPSkip == true && TRACKSkip == true) {
				if (patern_ < 8) {
					std::unique_ptr<Enemy> newRobot = std::make_unique<Enemy>();
					newRobot->Initialize(ROTATION, POSITION, camera_.get(), TRACK, step);
					robot_.push_back(std::move(newRobot));
				}
				else {
					std::unique_ptr<BossEnemy> boss = std::make_unique<BossEnemy>();
					boss->Initialize(ROTATION, POSITION, camera_.get(), TRACK);
					boss_.push_back(std::move(boss));
					break;
				}
				POPSkip = false;
				TRACKSkip = false;
				ARIVESkip = false;
			}


		}

		if (patern_ < count) {
			break;
		}

		if (word.find("END") == 0) {
			getline(line_stram, word, ',');

			break;
		}

	}
}

void GameScene::CheckSameTrackPosition()
{
	for (std::unique_ptr<Enemy>& FirstEnemy : robot_) {
		for (std::unique_ptr<Enemy>& SecondEnemy : robot_) {
			if (FirstEnemy.get() != SecondEnemy.get()) {
				XMVECTOR FirstTrackPosition = FirstEnemy->GetTrackPos();
				XMVECTOR SecondTrackPosition = SecondEnemy->GetTrackPos();
				bool secondenemyarive = SecondEnemy->GetArive();
				bool firstenemyarive = FirstEnemy->GetArive();
				if (Action::GetInstance()->CompletelyTogetherXMVECTOR(FirstTrackPosition, SecondTrackPosition)) {
					otherenemyarive = true;
					SecondEnemy->WaitTrack(otherenemyarive);
				}
				if (secondenemyarive == false) {
					XMVECTOR firstenemytrack = FirstEnemy->CheckTrackPoint();
					XMVECTOR secondenemytrack = SecondEnemy->CheckTrackPoint();
					if (Action::GetInstance()->CompletelyTogetherXMVECTOR(firstenemytrack, secondenemytrack)) {
						otherenemyarive = false;
						FirstEnemy->WaitTrack(otherenemyarive);
					}
				}
			}
		}
	}
}

void GameScene::StartCameraWork()
{
	l_reticlepos = player_->GetPosition();
	if (GetCamWorkFlag == false && startflag_ == false) {

		XMVECTOR l_bodyworldpos = player_->GetBodyWorldPos();
		if (stanbyflag_ == false) {
			eyerot_.y = 180;
		}
		else if (stanbyflag_ == true && actioncount_ == 0) {
			Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
			//��������
			if (eyerot_.y <= 0) {
				eyerot_.y = 0;
				actiontimer_ += 0.2f;
				if (actiontimer_ > 5) {
					actiontimer_ = 5.0f;
					Action::GetInstance()->EaseOut(eyerot_.x, 95.0f);
				}
			}
			//��������
			if (eyerot_.x >= 90) {
				actiontimer_ = 0.0f;
				eyerot_.x = 90;
				actioncount_ = 1;
			}
		}
		if (actioncount_ == 1) {

			actiontimer_ += 0.15f;
			velocity_ = { 0.0f,0.67f,0.4f };
			if (actiontimer_ >= 5) {
				velocity_ = { 0.0f,-0.6f,0.0f };
				Action::GetInstance()->EaseOut(eyerot_.x, -5.0f);
			}
			if (eyerot_.x <= 0.0f) {
				eyerot_.x = 0.0f;
			}
			//�n�ʂɒ������Ƃ�
			if (l_bodyworldpos.m128_f32[1] <= 0.3f) {
				velocity_ = { 0.0f,0.0f,0.0f };
				l_reticlepos.m128_f32[1] = 0.0f;
				movieflag_ = true;
				GameState = MOVE;
			}
		}
		player_->SetBodyWorldPos(l_bodyworldpos);
	}

	if ((Mouse::GetInstance()->PushClick(1) || Mouse::GetInstance()->PushClick(0)) && stanbyflag_ == true && GetCamWorkFlag == false) {
		movieflag_ = true;
		actioncount_ = 100;
		eyerot_.x = 0;
		eyerot_.y = 0;
		velocity_ = { 0.0f,0.0f,0.0f };
		l_reticlepos = { 0.0f,-0.7f,13.0f };
		railcamera_->MatrixIdentity(l_reticlepos, eyerot_);
		GameState = MOVE;
	}

	if (stanbyflag_ == false) {
		actiontimer_ += 0.01f;
		if (actiontimer_ >= 1.0f) {
			actiontimer_ = 0.0f;
			stanbyflag_ = true;
		}
	}

	if (movieflag_ == false) {
		CurtainUpPos.y += 4;
		CurtainDownPos.y -= 4;
		SkipPos.y -= 2;

		if (CurtainUpPos.y >= 0) {
			CurtainUpPos.y = 0;
		}

		if (CurtainDownPos.y <= 620) {
			CurtainDownPos.y = 620;
		}

		if (SkipPos.y <= 620) {
			SkipPos.y = 620;
		}
	}
	else {
		CurtainUpPos.y -= 4;
		CurtainDownPos.y += 4;
		SkipPos.y += 4;

		if (CurtainUpPos.y <= -100) {
			CurtainUpPos.y = -100;
		}

		if (CurtainDownPos.y >= 720) {
			CurtainDownPos.y = 720;
			GetCamWorkFlag = true;
			startflag_ = true;
		}

		if (SkipPos.y >= 720) {
			SkipPos.y = 12000;
		}
	}

	curtainup_->SetPosition(CurtainUpPos);
	curtaindown_->SetPosition(CurtainDownPos);
	skip_->SetPosition(SkipPos);
}

void GameScene::PlayerMove()
{
	XMMATRIX l_cameramatrix;
	l_cameramatrix = railcamera_->GetWorld();
	cameravector_ = { 0.f,0.f,0.f,0.f };
	cameravector_ = XMVector3Transform(cameravector_, l_cameramatrix);

	if (MoveFlag == true) {
		MoveShakingHead();
		//MoveShakingHead();
		movespeed_ = 0.5f;
		(this->*MoveFuncTable[patern_])();
		pathrot_ = eyerot_;
	}
	else if (MoveFlag == false) {
		velocity_ = { 0.f,0.f,0.f };
	}
}

void GameScene::ScreenShake(float shakevalue)
{
	if (shakelimittime_ <= 1) {
		shakelimittime_ += 0.1f;
		if (shakingscreenflag_ == true) {
			shakingscreenvalue_ -= shakevalue;
			if (shakingscreenvalue_ <= -shakevalue) {
				shakingscreenflag_ = false;
			}
		}
		else {
			shakingscreenvalue_ += shakevalue;
			if (shakingscreenvalue_ >= shakevalue) {
				shakingscreenflag_ = true;
			}
		}
		eyerot_.x += shakingscreenvalue_;
	}
	else {
		shakingscreenflag_ = true;
		shakelimittime_ = 0;
		shakingscreenvalue_ = 0;
		eyerot_.x = 0;
		screenshakestate_ = NONE;
	}

}

void GameScene::MoveShakingHead()
{
	//���Z�ƌ��Z����ׂ̐�Βl
	const float EyeRotAbsouluteValue = 0.05f;
	//���]�����邽�߂̐�Βl
	const float AbsoluteValue = 0.5f;
	if (GameState == MOVE) {
		if (shake_ == true) {
			eyerot_.x += EyeRotAbsouluteValue;
			if (eyerot_.x >= AbsoluteValue) {
				shake_ = false;
			}
		}

		else {
			eyerot_.x -= EyeRotAbsouluteValue;
			if (eyerot_.x <= -AbsoluteValue) {
				shake_ = true;
			}
		}
	}

}

void GameScene::CheckcCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY, bool& CheckFlag)
{
	if ((check_Pos.x - radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x)
		&& (check_Pos.y - radY <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		CheckFlag = true;
	}
	else {
		CheckFlag = false;
	}
}

void GameScene::MoveStartBack()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[2] >= 20) {
		Action::GetInstance()->EaseOut(eyerot_.y, 185.0f);
		velocity_ = { 0.f,0.f,0.f };
		if (eyerot_.y >= 180) {
			StopFlag = true;
			MoveFlag = false;

		}
	}
}

void GameScene::MoveStartFront()
{
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		velocity_ = { 0, 0, 0 };
		MoveFlag = false;
		StopFlag = true;
	}
}

void GameScene::MovePointA()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[2] >= 40) {
		velocity_ = { 0.f,0.f,0.f };
		MoveFlag = false;
		StopFlag = true;
	}
}

void GameScene::MovePointALeft()
{
	Action::GetInstance()->EaseOut(eyerot_.y, -95.0f);
	if (eyerot_.y <= -90) {
		eyerot_.y = max(eyerot_.y, -90.0f);
		changerotation_ = eyerot_.y;
		velocity_ = { 0, 0, 0 };

		MoveFlag = false;
		StopFlag = true;
	}
}

void GameScene::MovePointB()
{
	Action::GetInstance()->EaseOut(eyerot_.y, 95.0f);
	if (eyerot_.y >= 90) {
		changerotation_ = 90;
		eyerot_.y = 90;
		velocity_ = { 0, 0, movespeed_ };
	}
	if (cameravector_.m128_f32[0] >= 30) {

		MoveFlag = false;
		StopFlag = true;
	}
}

void GameScene::MovePointC()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[0] >= 45) {

		MoveFlag = false;
		StopFlag = true;
		velocity_ = { 0, 0, 0 };
	}
}

void GameScene::MovePointCOblique()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[0] >= 50) {
		velocity_ = { 0, 0, 0 };
		Action::GetInstance()->EaseOut(eyerot_.y, 145.0f);
		if (eyerot_.y >= 135) {
			changerotation_ = 135;

			MoveFlag = false;
			StopFlag = true;
			velocity_ = { 0, 0, 0 };
		}
	}
}

void GameScene::MovePointCFront()
{
	if (cameravector_.m128_f32[0] <= 55) {
		velocity_ = { 0, 0, movespeed_ };
	}
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		changerotation_ = 0;

		MoveFlag = false;
		StopFlag = true;
		velocity_ = { 0, 0, 0 };
	}
}

void GameScene::GoalPointBack()
{
	actioncount_ = 0;
	velocity_ = { 0.f,0.f,movespeed_ };
	if (cameravector_.m128_f32[2] >= 80) {
		velocity_ = { 0.f,0.f,0.1f };
		if (cameravector_.m128_f32[2] >= 82) {
			velocity_ = { 0.0f,0.0f,0.0f };
			Action::GetInstance()->EaseOut(eyerot_.y, 185.0f);
			if (eyerot_.y >= 180) {
				changerotation_ = 0;

				MoveFlag = false;
				StopFlag = true;
				velocity_ = { 0, 0, 0 };
			}
		}
	}
}

void GameScene::GoalPoint()
{
	stanbyflag_ = false;
	velocity_ = { 0.f, 0.f, 0.1f };
	//ShakeHeadFlag = false;
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		changerotation_ = 0;
		eyerot_.y = 0;
	}
	if (cameravector_.m128_f32[2] >= 92) {
		velocity_ = { 0.f,0.05f,0.1f };
		if (cameravector_.m128_f32[2] >= 97) {
			velocity_ = { 0.0f,0.0f,0.0f };
			FringFlag = true;
			if (FringFlag == true) {
				velocity_ = { 0.0f,0.6683f,0.0f };
			}
		}
	}
	GetCamWorkFlag = false;
	movieflag_ = false;
	actioncount_ = 0;
}

void(GameScene::* GameScene::MoveFuncTable[])() {
	&GameScene::MoveStartBack,
		& GameScene::MoveStartFront,
		& GameScene::MovePointA,
		& GameScene::MovePointALeft,
		& GameScene::MovePointB,
		& GameScene::MovePointC,
		& GameScene::MovePointCOblique,
		& GameScene::MovePointCFront,
		& GameScene::GoalPointBack,
		& GameScene::GoalPoint
};