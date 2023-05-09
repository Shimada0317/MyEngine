#include "GameScene.h"
#include<cassert>
#include <iomanip>
#include"Action.h"
#include"Collision.h"
#include"Mouse.h"
#include"HelperMath.h"

#include"SceneManager.h"
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
	for (int i = 0; i < 3; i++) {
		searchlightdir_[i] = { 0,-10,0 };
		searchlightcolor_[i] = { 1.f,1.f,1.f };
	}
	searchlightpos_[0] = { 0, 20, 20 };
	searchlightpos_[1] = { 20, 10, 45 };
	searchlightpos_[2] = { 54,10,43 };
	//�g�����C�g���A�N�e�B�u
	lightgroupe_->SetSpotLightActive(0, true);
	lightgroupe_->SetSpotLightActive(1, true);
	lightgroupe_->SetSpotLightActive(2, true);
	lightgroupe_->SetSpotLightActive(3, true);
	lightgroupe_->SetSpotLightActive(4, true);
	//���C�g�Z�b�g
	Object3d::SetLight(light_.get());
	Object3d::SetLightGroup(lightgroupe_.get());
	//�J�����̐���
	camera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	//�X�v���C�g�̐���
	damageefectsprite_.reset(Sprite::SpriteCreate(Name::kDamageEffect, { 0.0f, 0.0f }, damageefectcolor_));
	clear_.reset(Sprite::SpriteCreate(kGameClear, { 0.0f,0.0f }));
	shot_.reset(Sprite::SpriteCreate(kShot, { 0.f,WinApp::window_height - 150 }));
	reticleforgameover_.reset(Sprite::SpriteCreate(kReticle, reticleposition_, reticlecolor_, spriteanchorpoint_));
	//�R���e�B�j���[��ʂ̐���
	continue_screen_ = make_unique<Continue>();
	continue_screen_->Create(yesposition_, noposition_);
	//�ŏ��ƍŌ�̃V�l�}�����o�̐���
	movie_ = make_unique<Movie>();
	movie_->Create();
	movie_->Disply();
	//�I�u�W�F�N�g�̐���
	heri_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	goal_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	hane_ = Object3d::Create(ModelManager::GetInstance()->GetModel(12));
	//�w�i�̃I�u�W�F�N�g�̐���
	common_background_ = make_unique<CommonBackground>();
	common_background_->Initialize();
	//�v���C���[�̐���
	player_ = make_unique<Player>();
	player_->Initalize(camera_.get());
	playerhp_ = player_->GetHp();
	oldhp_ = playerhp_;
	//���[���J�����̐���
	railcamera_ = make_unique<RailCamera>();
	railcamera_->MatrixIdentity(player_->GetPosition(), player_->GetRotation());
	railcamera_->Update(velocity_, eyerot_, camera_.get());
	//�I�[�f�B�I�̐���
	herifry_ = make_unique<Audio>();
	bgm_ = make_unique<Audio>();
	herifry_->Initialize();
	bgm_->Initialize();
	herifry_->LoadFile("Resources/Sound/SE/heriFry.wav", 0.8f);
	bgm_->LoopWave("Resources/Sound/BGM/Blinded.wav", 0.3f);
	//�Q�[���̔w�i�I�u�W�F�N�g
	game_background_ = make_unique<GameBackground>();
	game_background_->LoadBackgrounndPopData();
	//csv�̓ǂݍ���
	LoadEnemyPopData();
}
//�X�e�[�^�X�Z�b�g
void GameScene::StatusSet()
{
	//�_���[�W�G�t�F�N�g�̐F
	damageefectsprite_->SetColor(damageefectcolor_);
	//�f�敗���o�X�e�[�^�X
	movie_->StatusSet();
	//Hp�o�[
	player_->SetHp(playerhp_);
	playerhp_ = player_->GetHp();
	//�w���R�v�^�[�̃X�e�[�^�X
	heri_->SetPosition(heripos_);
	heri_->SetScale(heriscl_);
	heri_->SetRotation({ 0.0f,180.0f,0.0f });
	goal_->SetPosition(goalpos_);
	goal_->SetScale(goalscl_);
	goal_->SetRotation({ 0.0f,270.0f,0.0f });
	//�w���̃v���y����
	hane_->SetRotation({ 0.0f,heriy_,0.0f });
	if (startmovieflag_ == false) {
		hane_->SetPosition(heripos_);
		hane_->SetScale(heriscl_);
	}
	else {
		hane_->SetPosition(goalpos_);
		hane_->SetScale(goalscl_);
	}
	//�t�B�[���h�S�̂��Ƃ炷�X�e�[�^�X
	lightgroupe_->SetSpotLightDir(0, XMVECTOR({ fieldspotlightdir_.x, fieldspotlightdir_.y, fieldspotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(0, fieldspotlightpos_);
	lightgroupe_->SetSpotLightColor(0, fieldspotlightcolor_);
	lightgroupe_->SetSpotLightAtten(0, fieldspotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(0, fieldspotlightfactorangle_);
	//�X�^�[�g�n�_�̃X�e�[�^�X
	lightgroupe_->SetSpotLightDir(1, XMVECTOR({ playerspotlightdir_.x, playerspotlightdir_.y, playerspotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(1, playerspotlightpos_);
	lightgroupe_->SetSpotLightColor(1, playerspotlightcolor_);
	lightgroupe_->SetSpotLightAtten(1, playerspotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(1, playerspotlightfactorangle_);
	//�t�B�[���h���𓮂����C�g�̃X�e�[�^�X
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
	const XMFLOAT4 BillColor = { 0.8f,0.6f,0.3f,1.0f };
	if (gamestate_ == MOVE) {
		velocity_ = XMVector3TransformNormal(velocity_, player_->GetBodyMatrix());
	}
	//�I�u�W�F�N�g�̍X�V����
	heri_->Update({ 0.7f,0.7f,0.6f,1.0f });
	goal_->Update({ 0.7f,0.7f,0.6f,1.0f });
	hane_->Update({ 0.0f,0.0f,0.0f,1.0f });
	common_background_->Update();
	game_background_->UpdateBackgroudPopCommands();
	game_background_->Update();
	//�v���C���[�̍X�V����
	player_->Update(camera_.get(), (Phase)patern_, passrot_,gamestate_,START);
	//���[���J�����̍X�V
	railcamera_->Update(velocity_, eyerot_, camera_.get());
}
//�Q�[���V�[���̍X�V����
void GameScene::Update()
{
	const float add_position_ = 3.1f;
	const float add_rotation_ = 15.f;
	heripos_.m128_f32[2] += add_position_;
	heriy_ += add_rotation_;
	//�J�����̉��o�O�̃t�F�[�h
	FadeIn();
	//�J�n���̃J�������[�N
	StartProcess();
	//�v���C���[�̈ړ�
	MoveProcess();
	//�퓬���̏���
	FightProcess();
	//�Q�[���I�[�o�[���̏���
	GameOverProcess();
	//�Q�[���N���A���̏���
	GameClearProcesss();
	//�S�[���ɂ��Ă��Ȃ��Ƃ��X�V�𑱂���
	StatusSet();
	if (gamestate_ == NONE || gamestate_ == START || gamestate_ == FIGHT || gamestate_ == MOVE) {
		//�X�|�b�g���C�g�̓����̏���
		SpotLightMove();
		AllUpdata();
		Action::GetInstance()->ScreenShake(shake_addvalue_, 0.1f, eyerot_, shakingstartflag_);
	}

	camera_->RecalculationMatrix();
	posteffect_->Update(postcol_);
	lightgroupe_->Update();
}
//�n�܂�̉��o�̏���
void GameScene::StartProcess()
{
	if (gamestate_ != GamePhase::START) { return; }
	l_reticlepos = player_->GetPosition();
	XMVECTOR l_bodyworldpos = player_->GetBodyWorldPos();
	//�Q�[���V�[���ɑJ�ڌ�
	if (movie_sequence_ == MovieSequence::ACTION && heripos_.m128_f32[2] >= 20) {
		backobjflag_ = false;
		startmovieflag_ = true;
		movie_sequence_ = MovieSequence::TURNAROUND;
	}
	//��������
	if (movie_sequence_ == MovieSequence::TURNAROUND) {
		Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
		if (eyerot_.y <= 0) {
			eyerot_.y = 0;
			movie_sequence_ = MovieSequence::FACELOWER;
		}
	}
	//��������
	if (movie_sequence_ == MovieSequence::FACELOWER) {
		actiontimer_ += 0.2f;
		if (actiontimer_ > 5) {
			actiontimer_ = 5.0f;
			Action::GetInstance()->EaseOut(eyerot_.x, 95.0f);
			if (eyerot_.x >= 90) {
				actiontimer_ = 0.0f;
				eyerot_.x = 90;
				movie_sequence_ = MovieSequence::JUMP;
			}
		}
	}
	//����
	if (movie_sequence_ ==MovieSequence::JUMP) {
		actiontimer_ += 0.15f;
		velocity_ = { 0.0f,0.67f,0.4f };
		if (actiontimer_ >= 5) {
			velocity_ = { 0.0f,-0.6f,0.0f };
			Action::GetInstance()->EaseOut(eyerot_.x, -5.0f);
		}
		if (eyerot_.x <= 0.0f) {
			eyerot_.x = 0.0f;
			
		}
		if (l_bodyworldpos.m128_f32[1] <= 0.9f) {
			l_bodyworldpos.m128_f32[1] = 0.9f;
			velocity_ = { 0.0f,0.0f,0.0f };
			movie_sequence_ = MovieSequence::LANDING;
		}
	}
	//�n�ʂɒ������Ƃ�
	if (movie_sequence_ == MovieSequence::LANDING) {
		l_reticlepos = { 0.0f,-0.7f,13.0f };
		railcamera_->MatrixIdentity(l_reticlepos, eyerot_);
		movie_sequence_ = MovieSequence::FINISH;
	}
	//���o�X�L�b�v
	SkipStartMovie(l_bodyworldpos);
	player_->SetBodyWorldPos(l_bodyworldpos);
	if (movie_sequence_ != MovieSequence::FINISH) { return; }
	//�f�敗���o�̕s��
	movie_->Invisible(gamestate_, MOVE);
}
//�ړ����̏���
void GameScene::MoveProcess()
{
	if (gamestate_ != MOVE) { return; }
	XMMATRIX l_cameramatrix;
	l_cameramatrix = railcamera_->GetWorld();
	cameravector_ = { 0.f,0.f,0.f,0.f };
	cameravector_ = XMVector3Transform(cameravector_, l_cameramatrix);
	//�����Ă���Ƃ��̂悤�Ȏ�𓮂���
	if (movie_sequence_ == FINISH) {
		Action::GetInstance()->MoveShakingHead(eyerot_);
	}
	(this->*MoveFuncTable[patern_])();
	//�v���C���[�ɓn���p�x
	passrot_ = eyerot_;
	if (!stopflag_) { return; }
	UpdataEnemyPopCommands();
	patern_ += 1;
	stopflag_ = false;
}
//�퓬���̏���
void GameScene::FightProcess()
{
	if (gamestate_ != FIGHT) { return; }
	//�_���[�W��H������Ƃ��̏���
	DamageProcess();
	XMFLOAT2 Player2DPos = player_->GetRetPosition();
	bool PlayerBulletShot_F = player_->GetBulletShot();
	if (PlayerBulletShot_F == true) {
		shake_addvalue_ = 1.f;
		shakingstartflag_ = true;
	}
	//�G�̍X�V����
	for (std::unique_ptr<NormalEnemy>& NormalEnemy : robot_) {
		NormalEnemy->Update(Player2DPos, playerhp_, PlayerBulletShot_F);
	}
	for (std::unique_ptr<ThrowEnemy>& ThrowEnemy : throw_) {
		ThrowEnemy->Update(Player2DPos, playerhp_, PlayerBulletShot_F);
	}

	//�{�X�̍X�V����
	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Update(Player2DPos, playerhp_, PlayerBulletShot_F);
	}
	//�ǔ��悪��������̓G�̏���
	CheckSameTrackPosition();
	player_->SetBulletShot(PlayerBulletShot_F);
	//�S�Ă̓G��|��
	KilledAllEnemy();
}
//�Q�[���I�[�o�[���̏���
void GameScene::GameOverProcess()
{
	if (gamestate_ != CONTINUE) { return; }
	Mouse::GetInstance()->MouseMoveSprite(reticleposition_);
	reticleforgameover_->SetPosition(reticleposition_);
	const float radx_ = 100;
	const float rady_ = 50;
	const XMFLOAT4 color_red_{ 1.f,0.f,0.f,1.f };
	if (gamestate_ == CONTINUE) {
		postcol_.x = 0;
		Collision::GetInstance()->ToggleFlagInClick(reticleposition_, yesposition_, radx_, rady_, yescursorinflag_);
		Collision::GetInstance()->ToggleFlagInClick(reticleposition_, noposition_, radx_, rady_, nocursorinflag_);
		//Yes�̕����ɃJ�[�\�������킹���Ƃ�
		if (yescursorinflag_ == true) {
			yescolor_ = color_red_;
			if (Mouse::GetInstance()->PushClick(0)) {
				BaseScene* scene_ = new GameScene(sceneManager_);
				sceneManager_->SetNextScene(scene_);
			}
		}
		else {
			yescolor_ = { 1.f,1.f,1.f,1.f };
		}
		//No�̕����ɃJ�[�\�������킹���Ƃ�
		if (nocursorinflag_ == true) {
			nocolor_ = color_red_;
			if (Mouse::GetInstance()->PushClick(0)) {
				BaseScene* scene_ = new TitleScene(sceneManager_);
				sceneManager_->SetNextScene(scene_);
			}
		}
		else {
			nocolor_ = { 1.f,1.f,1.f,1.f };
		}
		continue_screen_->ChangeColor(yescolor_, nocolor_);
	}
}
//�Q�[���N���A���̏���
void GameScene::GameClearProcesss()
{
	//�S�[���ɒ�������N���A��ʂ�\��
	if (gamestate_ != CLEAR) { return; }
	if (Mouse::GetInstance()->PushClick(0)) {
		BaseScene* scene_ = new TitleScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}
}
//�J�ڂ悤�̃t�F�[�h
void GameScene::FadeIn()
{
	if (gamestate_ != GamePhase::NONE) { return; }
	const float AddPosetEfectColor = 0.05f;
	postcol_.x += AddPosetEfectColor;
	postcol_.y += AddPosetEfectColor;
	postcol_.z += AddPosetEfectColor;
	
	if (postcol_.x >= 0.0f) {
		postcol_.x = 0.0f;
		postcol_.y = 0.0f;
		postcol_.z = 0.0f;
		gamestate_ = START;
	}

}
//���C�g�̓���
void GameScene::SpotLightMove()
{
	const float duration_ = 1;
	const float startpointz_ = 50.f;
	const float startpointx_ = -30;
	const float startpointz2_ = 50.f;
	const float endpointz_ = 0.f;
	const float endpointx_ = 30;
	const float endpointz2_ = 90.0f;
	const float startcolor_ = -0.5f;
	const float startcolorred_ = 0.0f;
	const float endcolor_ = 0.0f;
	const float endcolorred_ = 0.8f;

	if (easing_ == false) {
		easingwaittimer_ += 0.1f;
		if (easingwaittimer_ >= 1) {
			easing_ = true;
			easingwaittimer_ = 0.f;
		}
	}
	else if (easing_ == true) {
		if (easingchange_ == false) {
			if (duration_ > time_) {
				time_ += 0.01f;
			}
		}
		else {
			if (-duration_ < time_) {
				time_ -= 0.01f;
			}
		}
	}

	if (lightdireasingchange_ == false) {
		lightdireasingtime_ += 0.05f;
	}
	else {
		lightdireasingtime_ -= 0.05f;
	}

	searchlightdir_[0].x = Action::GetInstance()->EasingOut(lightdireasingtime_, 5 - 0);
	searchlightdir_[1].z = Action::GetInstance()->EasingOut(lightdireasingtime_, 5 - 0);
	searchlightdir_[2].x = Action::GetInstance()->EasingOut(lightdireasingtime_, 5 - 0);

	searchlightdir_[0].z = Action::GetInstance()->EasingOut(time_, endpointz_ - startpointz_);
	searchlightdir_[1].x = Action::GetInstance()->EasingOut(time_, endpointx_ - startpointx_);
	searchlightdir_[2].z = Action::GetInstance()->EasingOut(time_, endpointz2_ - startpointz2_);



	if (patern_ == GOALPOINT) {
		if (colortime_ >= 0) {
			colortime_ -= 0.01f;
		}
		if (colortimered_ <= 1) {
			colortimered_ += 0.01f;
		}

		fieldspotlightcolor_.x = Action::GetInstance()->EasingOut(colortimered_, endcolorred_ - startcolorred_) + 0.9f;
		fieldspotlightcolor_.y = Action::GetInstance()->EasingOut(colortime_, endcolor_ - startcolor_);
	}

	if (spotlightpositionchange_ == false) {
		if (time_ >= 1.f) {
			spotlightpositionchange_ = true;
			easing_ = false;
			easingchange_ = true;
		}
	}
	else {
		if (time_ <= -1.f) {
			spotlightpositionchange_ = false;
			easing_ = false;
			easingchange_ = false;
		}
	}
}
//�_���[�W��H������Ƃ��̏���
void GameScene::DamageProcess()
{
	if (playerhp_ > 0) {
		//�_���[�W��H��������Ƃ�
		if (oldhp_ > playerhp_) {
			posteffectonflag_ = true;
			damagehitflag_ = true;
			damageefectcolor_.w = 1;
			oldhp_ = playerhp_;
			shake_addvalue_ = 4.5f;
			shakingstartflag_ = true;
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
		postcol_.x += 0.01f;
		if (postcol_.x >= 2.0f) {
			gamestate_ = CONTINUE;
		}
	}
	//�_���[�W��H������Ƃ�
	if (damagehitflag_ == true) {
		damageefectcolor_.w -= 0.02f;
		if (damageefectcolor_.w <= 0) {
			damagehitflag_ = false;
		}
	}
}
//�G�̃X�e�[�^�X��csv�ǂݍ���
void GameScene::LoadEnemyPopData()
{
	ifstream file;
	file.open("Resources/LoadEnemy.csv");
	assert(file.is_open());

	enemypopcommands_ << file.rdbuf();

	file.close();
}
//�G�̏o������
void GameScene::UpdataEnemyPopCommands()
{

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
			//�p�x�̎擾
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
					std::unique_ptr<ThrowEnemy> newRobot = std::make_unique<ThrowEnemy>();
					newRobot->Initialize(ROTATION, POSITION, camera_.get(), TRACK);
					throw_.push_back(std::move(newRobot));
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
//�G���m�̒ǔ��悪����Ă���Ƃ�
void GameScene::CheckSameTrackPosition()
{
	for (std::unique_ptr<NormalEnemy>& FirstEnemy : robot_) {
		for (std::unique_ptr<NormalEnemy>& SecondEnemy : robot_) {
			if (FirstEnemy.get() != SecondEnemy.get()) {
				XMVECTOR FirstTrackPosition = FirstEnemy->GetTrackPos();
				XMVECTOR SecondTrackPosition = SecondEnemy->GetTrackPos();
				bool secondenemyarive = SecondEnemy->GetArive();
				bool firstenemyarive = FirstEnemy->GetArive();
				if (Action::GetInstance()->CompletelyTogetherXMVECTOR(FirstTrackPosition, SecondTrackPosition)) {
					otherenemyarive_ = true;
					SecondEnemy->WaitTrack(otherenemyarive_);
				}
				if (secondenemyarive == false) {
					XMVECTOR firstenemytrack = FirstEnemy->CheckTrackPoint();
					XMVECTOR secondenemytrack = SecondEnemy->CheckTrackPoint();
					if (Action::GetInstance()->CompletelyTogetherXMVECTOR(firstenemytrack, secondenemytrack)) {
						otherenemyarive_ = false;
						FirstEnemy->WaitTrack(otherenemyarive_);
					}
				}
			}
		}
	}
}
//�\������Ă���S�Ă̓G��|������
void GameScene::KilledAllEnemy()
{
	throw_.remove_if([](std::unique_ptr<ThrowEnemy>& throwrobot) {
		return throwrobot->IsDead();
		});
	robot_.remove_if([](std::unique_ptr<NormalEnemy>& robot) {
		return robot->IsDead();
		});
	boss_.remove_if([](std::unique_ptr<BossEnemy>& boss) {
		return boss->IsDead();
		});
	//�ڂ̑O�̓G��S�ē|�������v���C���[�𓮂���
	if (robot_.empty() && boss_.empty()&&throw_.empty()) {
		gamestate_ = MOVE;
	}
}
//�n�܂�̉��o�X�L�b�v����
void GameScene::SkipStartMovie(XMVECTOR& bodypos)
{
	if ((Mouse::GetInstance()->PushClick(1) || Mouse::GetInstance()->PushClick(0))) {
		eyerot_.x = 0;
		eyerot_.y = 0;
		velocity_ = { 0.0f,0.0f,0.0f };
		l_reticlepos = { 0.0f,-0.7f,13.0f };
		railcamera_->MatrixIdentity(l_reticlepos, eyerot_);
		bodypos.m128_f32[1] = 0.9f;
		movie_sequence_ = MovieSequence::FINISH;
	}
}
//1��ڂ̐퓬�n�_
void GameScene::MoveStartBack()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[2] >= 20) {
		Action::GetInstance()->EaseOut(eyerot_.y, 185.0f);
		velocity_ = { 0.f,0.f,0.f };
		if (eyerot_.y >= 180) {
			gamestate_ = FIGHT;
			stopflag_ = true;
		}
	}
}
//2��ڂ̐퓬�n�_
void GameScene::MoveStartFront()
{
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		velocity_ = { 0, 0, 0 };
		gamestate_ = FIGHT;
		stopflag_ = true;
	}
}
//3��ڂ̐퓬�n�_
void GameScene::MovePointA()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[2] >= 40) {
		velocity_ = { 0.f,0.f,0.f };
		gamestate_ = FIGHT;
		stopflag_ = true;
	}
}
//4��ڂ̐퓬�n�_
void GameScene::MovePointALeft()
{
	Action::GetInstance()->EaseOut(eyerot_.y, -95.0f);
	if (eyerot_.y <= -90) {
		eyerot_.y = max(eyerot_.y, -90.0f);
		gamestate_ = FIGHT;
		changerotation_ = eyerot_.y;
		velocity_ = { 0, 0, 0 };
		stopflag_ = true;
	}
}
//5��ڂ̐퓬�n�_
void GameScene::MovePointB()
{
	Action::GetInstance()->EaseOut(eyerot_.y, 95.0f);
	if (eyerot_.y >= 90) {
		changerotation_ = 90;
		eyerot_.y = 90;
		velocity_ = { 0, 0, movespeed_ };
	}
	if (cameravector_.m128_f32[0] >= 30) {
		gamestate_ = FIGHT;
		stopflag_ = true;
		velocity_ = { 0, 0, 0 };
	}
}
//6��ڂ̐퓬�n�_
void GameScene::MovePointC()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[0] >= 45) {
		gamestate_ = FIGHT;
		stopflag_ = true;
		velocity_ = { 0, 0, 0 };
	}
}
//7��ڂ̐퓬�n�_
void GameScene::MovePointCOblique()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[0] >= 50) {
		velocity_ = { 0, 0, 0 };
		Action::GetInstance()->EaseOut(eyerot_.y, 145.0f);
		if (eyerot_.y >= 135) {
			gamestate_ = FIGHT;
			changerotation_ = 135;
			stopflag_ = true;
			velocity_ = { 0, 0, 0 };
		}
	}
}
//8��ڂ̐퓬�n�_
void GameScene::MovePointCFront()
{
	if (cameravector_.m128_f32[0] <= 55) {
		velocity_ = { 0, 0, movespeed_ };
	}
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		gamestate_ = FIGHT;
		changerotation_ = 0;
		stopflag_ = true;
		velocity_ = { 0, 0, 0 };
	}
}
//�{�X�̐퓬�n�_
void GameScene::GoalPointBack()
{
	velocity_ = { 0.f,0.f,movespeed_ };
	if (cameravector_.m128_f32[2] >= 80) {
		velocity_ = { 0.f,0.f,0.1f };
		if (cameravector_.m128_f32[2] >= 82) {
			velocity_ = { 0.0f,0.0f,0.0f };
			Action::GetInstance()->EaseOut(eyerot_.y, 185.0f);
			if (eyerot_.y >= 180) {
				gamestate_ = FIGHT;
				changerotation_ = 0;
				stopflag_ = true;
				velocity_ = { 0, 0, 0 };
			}
		}
	}
}
//�S�[��
void GameScene::GoalPoint()
{
	movie_sequence_ = MovieSequence::ACTION;
	movie_->Disply();
	velocity_ = { 0.f, 0.f, 0.1f };
	//��������
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		changerotation_ = 0;
		eyerot_.y = 0;
	}
	//�w���Ɍ�����
	if (cameravector_.m128_f32[2] >= 92) {
		velocity_ = { 0.f,0.05f,0.1f };
		//�w���ɏ��
		if (cameravector_.m128_f32[2] >= 97) {
			velocity_ = { 0.0f,0.0f,0.0f };
			FringFlag = true;
			if (FringFlag == true) {
				velocity_ = { 0.0f,0.6683f,0.0f };
				//�S�[���ɒ������Ƃ�
				if (goalpos_.m128_f32[1] >= 100) {
					gamestate_ = CLEAR;

				}
			}
		}
	}
}
//�ړ��p�̊֐��|�C���^
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
//�I�u�W�F�N�g�̕`�揈��
void GameScene::ObjDraw(DirectXCommon* dxCommon)
{
	////�I�u�W�F�N�g�O����
	Object3d::PreDraw(dxCommon->GetCmdList());
	common_background_->Draw();
	game_background_->Draw();
	goal_->Draw();
	hane_->Draw();
	if (backobjflag_) {
		heri_->Draw();
	}
	if (movie_sequence_==MovieSequence::FINISH) {
		player_->ObjDraw();
	}
	player_->ParticleDraw(dxCommon->GetCmdList());
	////�I�u�W�F�N�g�㏈��
	Object3d::PostDraw();
	for (std::unique_ptr<NormalEnemy>& robot : robot_) {
		robot->Draw(dxCommon);
	}
	for (std::unique_ptr<ThrowEnemy>& throwrobot : throw_) {
		throwrobot->Draw(dxCommon);
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
	if (damagehitflag_) {
		damageefectsprite_->Draw();
	}
	if (gamestate_ == CLEAR) {
		clear_->Draw();
	}
	if (gamestate_ == CONTINUE) {
		continue_screen_->Draw();
		reticleforgameover_->Draw();
	}
	movie_->Draw();
	if ((gamestate_ == FIGHT || gamestate_ == MOVE)&&movie_sequence_==MovieSequence::FINISH) {
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

	ImGui::SliderInt("Actioncount", &actioncount_, -100, 100);
	ImGui::SliderFloat("Actiontimer", &actiontimer_, -100.0f, 100.0f);
	ImGui::SliderFloat("eyerot", &passrot_.y, -180.0f, 180.0f);
	ImGui::SliderFloat("eyerotY", &eyerot_.y, -180.0f, 180.0f);

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
	continue_screen_.reset();
	clear_.reset();
	shot_.reset();
	player_.reset();
}