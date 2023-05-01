#include "GameScene.h"
#include<cassert>
#include <iomanip>
#include"Action.h"
#include"Collision.h"
#include"Mouse.h"
#include"HelperMath.h"

#include"SceneManager.h"


using namespace DirectX;


//コンストラクタ
GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//初期化処理
void GameScene::Initialize(DirectXCommon* dxComon)
{
	//ポストエフェクトの生成
	posteffect_ = make_unique<PostEffect>();
	posteffect_->Initialize();
	posteffect_->Update(postcol_);
	//ライトの生成
	light_ = Light::Create();
	lightgroupe_ = LightGroup::Create();
	//ライトセット
	Object3d::SetLight(light_.get());
	Object3d::SetLightGroup(lightgroupe_.get());
	//カメラの生成
	camera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	//スプライトの生成
	damageefectsprite_.reset(Sprite::SpriteCreate(Name::kDamageEffect, { 0.0f, 0.0f }, damageefectcolor_));
	clear_.reset(Sprite::SpriteCreate(kGameClear, { 0.0f,0.0f }));
	conteniu_.reset(Sprite::SpriteCreate(kGameOver, { 0.0f,0.0f }));
	shot_.reset(Sprite::SpriteCreate(kShot, { 0.f,WinApp::window_height - 150 }));
	reticleforgameover_.reset(Sprite::SpriteCreate(kReticle, reticleposition_, reticlecolor_, spriteanchorpoint_));
	yes_.reset(Sprite::SpriteCreate(kYes, yesposition_, yescolor_, spriteanchorpoint_));
	no_.reset(Sprite::SpriteCreate(kNo, noposition_, nocolor_, spriteanchorpoint_));
	hart_.reset(Sprite::SpriteCreate(kHart, hartposition_, { 1.f,1.f,1.f,1.f }, spriteanchorpoint_));
	curtainup_.reset(Sprite::SpriteCreate(Name::kCurtain, curtainuppos_));
	curtaindown_.reset(Sprite::SpriteCreate(Name::kCurtain, curtaindownpos_));
	skip_.reset(Sprite::SpriteCreate(Name::kSkip, skippos_));
	for (int i = 0; i < 5; i++) {
		lifecount_[i].reset(Sprite::SpriteCreate(i, hartposition_));
	}
	//オブジェクトの生成
	heri_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	goal_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	hane_ = Object3d::Create(ModelManager::GetInstance()->GetModel(12));
	//背景のオブジェクトの生成
	common_background_ = make_unique<CommonBackground>();
	common_background_->Initialize();
	
	player_ = make_unique<Player>();
	player_->Initalize(camera_.get());
	playerhp_ = player_->GetHp();
	oldhp_ = playerhp_;

	railcamera_ = make_unique<RailCamera>();
	railcamera_->MatrixIdentity(player_->GetPosition(), player_->GetRotation());
	railcamera_->Update(velocity_, eyerot_, camera_.get());

	bgm_ = make_unique<Audio>();
	bgm_->Initialize();
	bgm_->LoopWave("Resources/Sound/BGM/Blinded.wav", 0.3f);

	herifry_ = make_unique<Audio>();
	herifry_->Initialize();
	herifry_->LoadFile("Resources/Sound/SE/heriFry.wav", 0.8f);

	game_background_ = make_unique<GameBackground>();
	game_background_->LoadBackgrounndPopData();
	

	for (int i = 0; i < 3; i++) {
		searchlightdir_[i] = { 0,-10,0 };
		searchlightcolor_[i] = { 1.f,1.f,1.f };
	}
	searchlightpos_[0] = { 0, 20, 20 };
	searchlightpos_[1] = { 20, 10, 45 };
	searchlightpos_[2] = { 54,10,43 };
	for (int i = 0; i < 2; i++) {
		lightpositionchangex_[i] = false;
		lightpositionchangez_[i] = false;
	}

	lightgroupe_->SetSpotLightActive(0, true);
	lightgroupe_->SetSpotLightActive(1, true);
	lightgroupe_->SetSpotLightActive(2, true);
	lightgroupe_->SetSpotLightActive(3, true);
	lightgroupe_->SetSpotLightActive(4, true);
	originalsize_ = hartsize_;

	curtainup_->SetSize(curtainsize_);
	curtaindown_->SetSize(curtainsize_);
	LoadEnemyPopData();
}

//ステータスセット
void GameScene::StatusSet()
{
#pragma region 後で必要な変数の追加ごこのコメントを消す
	//変動するカウンター
	for (int i = 0; i < 5; i++) {
		lifecount_[i]->SetSize({ 80,80 });
		lifecount_[i]->SetPosition({ 1160,630 });
	}

	hart_->SetSize(hartsize_);
	hart_->SetPosition({ WinApp::window_width - 173,WinApp::window_height - 50 });
	//Hpバー
	player_->SetHp(playerhp_);
	playerhp_ = player_->GetHp();

	heri_->SetPosition(heripos_);
	heri_->SetScale(heriscl_);
	heri_->SetRotation({ 0.0f,180.0f,0.0f });

	hane_->SetRotation({ 0.0f,heriy_,0.0f });
	if (startmovieflag_ == false) {
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



	//フィールドの建物のステータスセット
	

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

	curtainup_->SetPosition(curtainuppos_);
	curtaindown_->SetPosition(curtaindownpos_);
	skip_->SetPosition(skippos_);
};

//オブジェクトなどの更新処理
void GameScene::AllUpdata()
{
	const XMFLOAT4 BillColor = { 0.8f,0.6f,0.3f,1.0f };

	if (getcamworkflag_ == true) {
		velocity_ = XMVector3TransformNormal(velocity_, player_->GetBodyMatrix());
	}
	//フィールドのビルの更新処理
	
	game_background_->UpdateBackgroudPopCommands();
	game_background_->Update();

	//プレイヤーの更新処理
	player_->Update(camera_.get(), (Phase)patern_, passrot_);
	railcamera_->Update(velocity_, eyerot_, camera_.get());
	common_background_->Update();
}

//ゲームシーンの更新処理
void GameScene::Update()
{
	//プレイヤーの移動
	PlayerMove();
	//映画のようなムービーの処理
	MovieProcess();
	//開始時のカメラワーク
	StartCameraWork();
	//スポットライトの動きの処理
	SpotLightMove();
	//ハートの鼓動の動き
	HeartBeat();

	if (gamestartflag_ == false) {
		FadeIn();
	}
	if (gamestartflag_ == true) {
		DamageProcess();
	}

	//ゴールに着いたとき
	if (goalpos_.m128_f32[1] >= 100) {
		clearflag_ = true;
		stopupdateflag_ = true;
	}
	//ゴールについていないとき更新を続ける
	StatusSet();
	if (stopupdateflag_ == false) {
		AllUpdata();
	}
	else if (stopupdateflag_ == true && clearflag_ == false)
	{
		GameOverProcess();
	}

	GameClearProcesss();

	if (screenshakestate_ != NONE) {
		shakingstartflag_ = true;
		screenshakestate_ = NONE;
	}
	ScreenShake(4.5f,0.1f);

#pragma region ActorからUpdate内の処理を持ってくる(後でこのコメントは消す)



	XMVECTOR velo = player_->GetVelocity();
	//SetVelocity(velo);

	heripos_.m128_f32[2] += herix_;

	if (heripos_.m128_f32[2] >= 20) {
		backobjflag_ = false;
		startmovieflag_ = true;
	}
	else {
		heriy_ += 15.0f;
	}

	if (getcamworkflag_ == true) {
		KilledAllEnemy();
		//プレイヤーが目的地点に着いたとき
		if (stopflag_ == true) {
			moveflag_ = false;
			UpdataEnemyPopCommands();
			patern_ += 1;
			stopflag_ = false;
		}
	}


	if (patern_ >= 6) {
		if (fringflag_ == true) {
			goalpos_.m128_f32[1] += velo.m128_f32[1];
		}
	}

	XMFLOAT2 Player2DPos = player_->GetRetPosition();
	bool PlayerBulletShot_F = player_->GetBulletShot();
	//敵の更新処理
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

//オブジェクトの描画処理
void GameScene::ObjDraw(DirectXCommon* dxCommon)
{
	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	//start_->Draw();
	common_background_->Draw();
	

	game_background_->Draw();

#pragma region ActorからDrawの処理を持ってくる(後で消す)
	goal_->Draw();
	hane_->Draw();
	if (backobjflag_ == true) {
		heri_->Draw();
	}
	if (gamestate_ != MOVIE) {
		player_->ObjDraw();
	}

	player_->ParticleDraw(dxCommon->GetCmdList());
#pragma endregion


	////オブジェクト後処理
	Object3d::PostDraw();
	for (std::unique_ptr<Enemy>& robot : robot_) {
		robot->Draw(dxCommon);
	}
	for (std::unique_ptr<BossEnemy>& boss : boss_) {
		boss->Draw(dxCommon);
	}
}

//スプライトの描画処理
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

	if (getcamworkflag_ == true) {
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

	if (getcamworkflag_ == false && startflag_ == false) {
		curtainup_->Draw();
		curtaindown_->Draw();
		skip_->Draw();

	}
	if (gamestate_ != MOVIE) {
		player_->SpriteDraw();
	}
	Sprite::PostDraw();
}

//ImgUiの描画処理
void GameScene::ImgDraw()
{

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Camera");
	
	ImGui::SliderInt("Actioncount", &actioncount_, -100, 100);
	ImGui::SliderFloat("Actiontimer", &actiontimer_, -100.0f, 100.0f);
	ImGui::SliderFloat("eyerot", &passrot_.y, -180.0f, 180.0f);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

//ポストエフェクトの描画処理
void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{
	posteffect_->PreDrawScene(dxCommon->GetCmdList());
	ObjDraw(dxCommon);

	posteffect_->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	posteffect_->Draw(dxCommon->GetCmdList());
	SpriteDraw(dxCommon);
	//描画後処理
	ImgDraw();
	player_->ImGuiDraw();
	dxCommon->PostDraw();
}

//描画処理
void GameScene::Draw(DirectXCommon* dxCommon)
{
	PostEffectDraw(dxCommon);
}

//終了処理
void GameScene::Finalize()
{
	conteniu_.reset();
	clear_.reset();
	shot_.reset();
	
}

void GameScene::FadeIn()
{
	const float AddPosetEfectColor = 0.05f;
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
	if (easing_ == false) {
		easingwaittimer_ += 0.1f;
		if (easingwaittimer_ >= 1) {
			easing_ = true;
			easingwaittimer_ = 0.f;
		}
	}

	if (easing_ == true) {
		if (easingchange_ == false) {
			duration_ = 1;
			if (duration_ > time_) {
				time_ += 0.01f;
			}
		}
		else {
			duration_ = -1;
			if (duration_ < time_) {
				time_ -= 0.01f;
			}
		}
	}
	if (changetimerflag_ == false) {
		lightaddposchangetimer_ += 0.01f;
	}
	else {
		lightaddposchangetimer_ -= 0.01f;
	}
	if (lightaddposchangetimer_ >= 1) {
		searchlightaddpos_ = HelperMath::GetInstance()->XMFLOAT3ChangeValue(searchlightaddpos_);
		changetimerflag_ = true;
	}
	else if (lightaddposchangetimer_ <= -1) {
		searchlightaddpos_ = HelperMath::GetInstance()->XMFLOAT3ChangeValue(searchlightaddpos_);
		changetimerflag_ = false;
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



	if (patern_ == 8 && moveflag_ == true) {
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

void GameScene::DamageProcess()
{
	if (playerhp_ > 0) {
		//ダメージを食らったたとき
		if (oldhp_ > playerhp_) {
			posteffectonflag_ = true;
			damagehitflag_ = true;
			damageefectcolor_.w = 1;
			oldhp_ = playerhp_;
			screenshakestate_ = DAMAGE;
			shakingscreenvalue_ = 0.f;
		}
		//画面を赤くするフラグが立った時
		if (posteffectonflag_ == true) {
			postcol_.x = 0.7f;
			if (postcol_.x >= 0.7f) {
				posteffectonflag_ = false;
			}
		}
		//画面を赤くするフラグが立っていない時
		if (posteffectonflag_ == false) {
			postcol_.x -= 0.05f;
			if (postcol_.x <= 0) {
				postcol_.x = 0;
			}
		}
	}
	//体力が0になったら
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
	Mouse::GetInstance()->MouseMoveSprite(reticleposition_);
	reticleforgameover_->SetPosition(reticleposition_);
	const float radx_ = 100;
	const float rady_ = 50;
	const XMFLOAT4 color_red_{ 1.f,0.f,0.f,1.f };
	if (dethflag_ == true) {
		postcol_.x = 0;
		Collision::GetInstance()->ToggleFlagInClick(reticleposition_, yesposition_, radx_, rady_, yescursorinflag_);
		Collision::GetInstance()->ToggleFlagInClick(reticleposition_, noposition_, radx_, rady_, nocursorinflag_);

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
		yes_->SetColor(yescolor_);
		no_->SetColor(nocolor_);
	}
}

void GameScene::GameClearProcesss()
{
	//ゴールに着いたらクリア画面を表示
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
	//待機処理
	if (waitflag_ == true) {
		waittime_--;
		if (waittime_ <= 0) {
			waitflag_ = false;
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
			//WAVEの要素
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
			//座標取得
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
			//追尾先の座標取得
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

				//待ち時間
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

void GameScene::KilledAllEnemy()
{
	robot_.remove_if([](std::unique_ptr<Enemy>& robot) {
		return robot->IsDead();
		});
	boss_.remove_if([](std::unique_ptr<BossEnemy>& boss) {
		return boss->IsDead();
		});
	//目の前の敵を全て倒した時プレイヤーを動かす
	if (robot_.empty() && boss_.empty()) {
		moveflag_ = true;
	}
}

void GameScene::StartCameraWork()
{
	l_reticlepos = player_->GetPosition();
	XMVECTOR l_bodyworldpos = player_->GetBodyWorldPos();
	if (getcamworkflag_) { return; }
	if (startflag_) { return; }

	if (!stanbyflag_) {
		eyerot_.y = 180;
	}
	else if (actioncount_ == 0) {
		Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
		//後ろを向く
		if (eyerot_.y <= 0) {
			eyerot_.y = 0;
			actiontimer_ += 0.2f;
			if (actiontimer_ > 5) {
				actiontimer_ = 5.0f;
				Action::GetInstance()->EaseOut(eyerot_.x, 95.0f);
			}
		}
		//下を向く
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
		//地面に着いたとき
		if (l_bodyworldpos.m128_f32[1] <= 0.3f) {
			velocity_ = { 0.0f,0.0f,0.0f };
			l_reticlepos.m128_f32[1] = 0.0f;
			gamestate_ = MOVE;
		}
	}
	player_->SetBodyWorldPos(l_bodyworldpos);




	if (stanbyflag_ == false) {
		actiontimer_ += 0.01f;
		if (actiontimer_ >= 1.0f) {
			actiontimer_ = 0.0f;
			stanbyflag_ = true;
		}
	}



	SkipStartMovie();


}

void GameScene::SkipStartMovie()
{
	if ((Mouse::GetInstance()->PushClick(1) || Mouse::GetInstance()->PushClick(0)) && stanbyflag_ == true && getcamworkflag_ == false) {
		actioncount_ = 100;
		eyerot_.x = 0;
		eyerot_.y = 0;
		velocity_ = { 0.0f,0.0f,0.0f };
		l_reticlepos = { 0.0f,-0.7f,13.0f };
		railcamera_->MatrixIdentity(l_reticlepos, eyerot_);
		gamestate_ = MOVE;
	}
}

void GameScene::PlayerMove()
{
	if (gamestate_ == MOVE) {
		XMMATRIX l_cameramatrix;
		l_cameramatrix = railcamera_->GetWorld();
		cameravector_ = { 0.f,0.f,0.f,0.f };
		cameravector_ = XMVector3Transform(cameravector_, l_cameramatrix);
		//敵を全て倒しムーブ状態になったら
		if (moveflag_ == true) {
			//歩いているときのような首を動かす
			MoveShakingHead();
			(this->*MoveFuncTable[patern_])();
			//プレイヤーに渡す角度
			passrot_ = eyerot_;
		}
	}
}


//画面揺れ
void GameScene::ScreenShake(float shakevalue, float shakingtime)
{
	if (shakingstartflag_ == true) {
		if (shakelimittime_ <= 1) {
			shakelimittime_ += shakingtime;
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
			shakingstartflag_ = false;
			shakingscreenvalue_ = 0;
			eyerot_.x = 0;
		}
	}
}



void GameScene::MoveShakingHead()
{
	//加算と減算する為の絶対値
	const float EyeRotAbsouluteValue = 0.05f;
	//反転させるための絶対値
	const float AbsoluteValue = 0.5f;
	if (gamestate_ == MOVE) {
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

void GameScene::HeartBeat()
{
	//タイマーの加算
	easingtimer_ += addtimer_;
	//反転フラグがtrueの時サイズを拡大
	if (reversflag_ == true) {
		hartsize_.x = Action::GetInstance()->EasingOut(easingtimer_, 40) + originalsize_.x;
		hartsize_.y = Action::GetInstance()->EasingOut(easingtimer_, 40) + originalsize_.y;
		if (easingtimer_ >= 1) {
			easingtimer_ = 0;
			originalsize_ = hartsize_;
			reversflag_ = false;
		}
	}
	//反転フラグがfalseの時サイズを縮小
	else {
		hartsize_.x = -Action::GetInstance()->EasingOut(easingtimer_, 40) + originalsize_.x;
		hartsize_.y = -Action::GetInstance()->EasingOut(easingtimer_, 40) + originalsize_.y;
		if (easingtimer_ >= 1) {
			easingtimer_ = 0;
			originalsize_ = hartsize_;
			reversflag_ = true;
		}
	}


	if (playerhp_ == 4) {
		addtimer_ = 0.01f;
	}
	else if (playerhp_ == 3) {
		addtimer_ = 0.05f;
	}
	else if (playerhp_ == 2) {
		addtimer_ = 0.1f;
	}
	else if (playerhp_ == 1) {
		addtimer_ = 0.5f;
	}
}

void GameScene::MovieProcess()
{
	if (gamestate_ == MOVIE) {
			curtainuppos_.y += 4;
			curtaindownpos_.y -= 4;
			skippos_.y -= 2;

			if (curtainuppos_.y >= 0) {
				curtainuppos_.y = 0;
			}

			if (curtaindownpos_.y <= 620) {
				curtaindownpos_.y = 620;
			}

			if (skippos_.y <= 620) {
				skippos_.y = 620;
			}
	}
	else {
			curtainuppos_.y -= 4;
			curtaindownpos_.y += 4;
			skippos_.y += 4;

			if (curtainuppos_.y <= -100) {
				curtainuppos_.y = -100;
			}

			if (curtaindownpos_.y >= 720) {
				curtaindownpos_.y = 720;
				getcamworkflag_ = true;
				startflag_ = true;
			}

			if (skippos_.y >= 720) {
				skippos_.y = 12000;
			}
		}
}

void GameScene::MoveProcess()
{
	if (gamestate_ == MOVE) {

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
			stopflag_ = true;
			moveflag_ = false;
		}
	}
}

void GameScene::MoveStartFront()
{
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		velocity_ = { 0, 0, 0 };
		moveflag_ = false;
		stopflag_ = true;
	}
}

void GameScene::MovePointA()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[2] >= 40) {
		velocity_ = { 0.f,0.f,0.f };
		moveflag_ = false;
		stopflag_ = true;
	}
}

void GameScene::MovePointALeft()
{
	Action::GetInstance()->EaseOut(eyerot_.y, -95.0f);
	if (eyerot_.y <= -90) {
		eyerot_.y = max(eyerot_.y, -90.0f);
		changerotation_ = eyerot_.y;
		velocity_ = { 0, 0, 0 };
		stopflag_ = true;
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
		stopflag_ = true;
		velocity_ = { 0, 0, 0 };
	}
}

void GameScene::MovePointC()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[0] >= 45) {
		stopflag_ = true;
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
			stopflag_ = true;
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
		stopflag_ = true;
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
				stopflag_ = true;
				velocity_ = { 0, 0, 0 };
			}
		}
	}
}

void GameScene::GoalPoint()
{
	gamestate_ = MOVIE;
	stanbyflag_ = false;
	velocity_ = { 0.f, 0.f, 0.1f };
	//後ろを向く
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		changerotation_ = 0;
		eyerot_.y = 0;
	}
	//ヘリに向かう
	if (cameravector_.m128_f32[2] >= 92) {
		velocity_ = { 0.f,0.05f,0.1f };
		//ヘリに乗る
		if (cameravector_.m128_f32[2] >= 97) {
			velocity_ = { 0.0f,0.0f,0.0f };
			FringFlag = true;
			if (FringFlag == true) {
				velocity_ = { 0.0f,0.6683f,0.0f };
			}
		}
	}
	getcamworkflag_ = false;
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