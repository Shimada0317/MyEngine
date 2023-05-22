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
	posteffect_->Update(postefectcolor_);
	//ライトの生成
	lightcontrol_ = make_unique<LightControl>();
	lightcontrol_->Initialize();
	//カメラの生成
	camera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);

	moviestaging_ = make_unique<MovieStaging>();
	//
	enemypop_ = make_unique<EnemyPop>();
	enemypop_->LoadCsv();
	//スプライトの生成
	damageefectsprite_.reset(Sprite::SpriteCreate(Name::kDamageEffect, { 0.0f, 0.0f }, damageefectcolor_));
	clear_.reset(Sprite::SpriteCreate(kGameClear, { 0.0f,0.0f }));
	shot_.reset(Sprite::SpriteCreate(kShot, { 0.f,WinApp::window_height - 150 }));
	reticleforgameover_.reset(Sprite::SpriteCreate(kReticle, reticleposition_, reticlecolor_, spriteanchorpoint_));
	//コンティニュー画面の生成
	continue_screen_ = make_unique<Continue>();
	continue_screen_->Create(yesposition_, noposition_);
	//最初と最後のシネマ風演出の生成
	movie_ = make_unique<Movie>();
	movie_->Create();
	movie_->Disply();
	//オブジェクトの生成
	heri_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	goal_ = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	hane_ = Object3d::Create(ModelManager::GetInstance()->GetModel(12));
	//背景のオブジェクトの生成
	common_background_ = make_unique<CommonBackground>();
	common_background_->Initialize();
	//プレイヤーの生成
	player_ = make_unique<Player>();
	player_->Initalize(camera_.get());
	playerhp_ = player_->GetHp();
	oldhp_ = playerhp_;
	//レールカメラの生成
	railcamera_ = make_unique<RailCamera>();
	railcamera_->MatrixIdentity(player_->GetPosition(), player_->GetRotation());
	railcamera_->Update(velocity_, eyerot_, camera_.get());
	//オーディオの生成
	herifry_ = make_unique<Audio>();
	bgm_ = make_unique<Audio>();
	herifry_->Initialize();
	bgm_->Initialize();
	herifry_->LoadFile("Resources/Sound/SE/heriFry.wav", 0.8f);
	bgm_->LoopWave("Resources/Sound/BGM/Blinded.wav", 0.3f);
	//ゲームの背景オブジェクト
	game_background_ = make_unique<GameBackground>();
	game_background_->LoadBackgrounndPopData();
}
//ステータスセット
void GameScene::StatusSet()
{
	//ダメージエフェクトの色
	damageefectsprite_->SetColor(damageefectcolor_);
	//映画風演出ステータス
	movie_->StatusSet();
	//Hpバー
	player_->SetHp(playerhp_);
	playerhp_ = player_->GetHp();
	//ヘリコプターのステータス
	heri_->SetPosition(heripos_);
	heri_->SetScale(heriscl_);
	heri_->SetRotation({ 0.0f,180.0f,0.0f });
	goal_->SetPosition(goalpos_);
	goal_->SetScale(goalscl_);
	goal_->SetRotation({ 0.0f,270.0f,0.0f });
	//ヘリのプロペラの
	hane_->SetRotation({ 0.0f,heriy_,0.0f });
	if (movie_sequence_ == MovieSequence::kAction) {
		hane_->SetPosition(heripos_);
		hane_->SetScale(heriscl_);
	}
	else {
		hane_->SetPosition(goalpos_);
		hane_->SetScale(goalscl_);
	}
};
//オブジェクトなどの更新処理
void GameScene::AllUpdata()
{
	if (gamestate_ == MOVE) {
		velocity_ = XMVector3TransformNormal(velocity_, player_->GetBodyMatrix());
	}
	//オブジェクトの更新処理
	heri_->Update({ 0.7f,0.7f,0.6f,1.0f });
	goal_->Update({ 0.7f,0.7f,0.6f,1.0f });
	hane_->Update({ 0.0f,0.0f,0.0f,1.0f });
	common_background_->Update();
	game_background_->UpdateBackgroudPopCommands();
	game_background_->Update();
	//プレイヤーの更新処理
	player_->Update(camera_.get(), (Phase)patern_, passrot_, gamestate_, START);
	//レールカメラの更新
	railcamera_->Update(velocity_, eyerot_, camera_.get());
}
//ゲームシーンの更新処理
void GameScene::Update()
{
	const float kAddPosition_ = 3.1f;
	const float kAddRotation_ = 15.f;
	heripos_.m128_f32[2] += kAddPosition_;
	heriy_ += kAddRotation_;
	//カメラの演出前のフェード
	FadeIn();
	//開始時のカメラワーク
	StartProcess();
	//プレイヤーの移動
	MoveProcess();
	//戦闘時の処理
	FightProcess();
	//ゲームオーバー時の処理
	GameOverProcess();
	//ゲームクリア時の処理
	GameClearProcesss();
	//ゴールについていないとき更新を続ける
	StatusSet();
	if (gamestate_ == NONE || gamestate_ == START || gamestate_ == FIGHT || gamestate_ == MOVE) {
		//スポットライトの動きの処理
		SpotLightMove();
		lightcontrol_->Update();
		AllUpdata();
		Action::GetInstance()->ScreenShake(shake_addvalue_, 0.1f, eyerot_, shakingstartflag_);
	}
	camera_->RecalculationMatrix();
	posteffect_->Update(postefectcolor_);
}
//始まりの演出の処理
void GameScene::StartProcess()
{
	if (gamestate_ != GamePhase::START) { return; }
	if (heripos_.m128_f32[2] < 20.f) { return; }
	//始まりのカメラ演出
	moviestaging_->StartMovie(player_.get(), eyerot_, velocity_, railcamera_.get());
	//カメラ演出の状態をGetする
	movie_sequence_ = moviestaging_->GetMovieSequence();
	//演出が終わっていないなら
	if (movie_sequence_ != MovieSequence::kFinish) { return; }
	//映画風演出の不可視
	movie_->Invisible(gamestate_, MOVE);
}
//移動時の処理
void GameScene::MoveProcess()
{
	//ゲームの状態が移動中の時
	if (gamestate_ != MOVE) { return; }
	XMMATRIX kCameraMatrix;
	kCameraMatrix = railcamera_->GetWorld();
	cameravector_ = { 0.f,0.f,0.f,0.f };
	cameravector_ = XMVector3Transform(cameravector_, kCameraMatrix);
	//歩いているときのような首を動かす
	moviestaging_->MoveShakingHead(eyerot_);
	(this->*MoveFuncTable[patern_])();
	//プレイヤーに渡す角度
	passrot_ = eyerot_;
	if (!stopflag_) { return; }
	enemypop_->PopEnemy( patern_, camera_.get());
	patern_ += 1;
	stopflag_ = false;
}
//戦闘時の処理
void GameScene::FightProcess()
{
	if (gamestate_ != FIGHT) { return; }
	//ダメージを食らったときの処理
	DamageProcess();
	XMFLOAT2 kPlayer2DPos = player_->GetRetPosition();
	bool kPlayerBulletShot = player_->GetBulletShot();
	if (kPlayerBulletShot == true) {
		shake_addvalue_ = 1.f;
		shakingstartflag_ = true;
	}
	//敵の更新処理
	enemypop_->Update(kPlayer2DPos, playerhp_, kPlayerBulletShot);
	//追尾先が被った時の敵の処理
	enemypop_->CheckSameTrackPosition();
	player_->SetBulletShot(kPlayerBulletShot);
	//全ての敵を倒す
	enemypop_->EnemyDead();
	//目の前の敵を全て倒した時プレイヤーを動かす
	if (enemypop_->KilledAllEnemy()) {
		gamestate_ = MOVE;
	}
}
//ゲームオーバー時の処理
void GameScene::GameOverProcess()
{
	//状態がコンティニューの時
	if (gamestate_ != CONTINUE) { return; }
	Mouse::GetInstance()->MouseMoveSprite(reticleposition_);
	reticleforgameover_->SetPosition(reticleposition_);
	const float kRadX = 100;
	const float kRadY = 50;
	const XMFLOAT4 kColorRed{ 1.f,0.f,0.f,1.f };
	postefectcolor_.x = 0;
	Collision::GetInstance()->ToggleFlagInClick(reticleposition_, yesposition_, kRadX, kRadY, yescursorinflag_);
	Collision::GetInstance()->ToggleFlagInClick(reticleposition_, noposition_, kRadX, kRadY, nocursorinflag_);
	//Yesの文字にカーソルを合わせたとき
	if (yescursorinflag_ == true) {
		yescolor_ = kColorRed;
		if (Mouse::GetInstance()->PushClick(0)) {
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
	else {
		yescolor_ = { 1.f,1.f,1.f,1.f };
	}
	//Noの文字にカーソルを合わせたとき
	if (nocursorinflag_ == true) {
		nocolor_ = kColorRed;
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
//ゲームクリア時の処理
void GameScene::GameClearProcesss()
{
	//ゴールに着いたらクリア画面を表示
	if (gamestate_ != CLEAR) { return; }
	if (Mouse::GetInstance()->PushClick(0)) {
		BaseScene* scene_ = new TitleScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}
}
//遷移ようのフェード
void GameScene::FadeIn()
{
	//ゲームが始まる前
	if (gamestate_ != GamePhase::NONE) { return; }
	const float kAddPosetEfectColor = 0.05f;
	Action::GetInstance()->ColorUp(postefectcolor_, kAddPosetEfectColor);
	if (postefectcolor_.x >= 0.0f) {
		postefectcolor_.x = 0.0f;
		postefectcolor_.y = 0.0f;
		postefectcolor_.z = 0.0f;
		postefectcolor_.w = 1.f;
		gamestate_ = START;
	}

}
//ライトの動き[ライトクラス内で動きの地点だけで管理する]
void GameScene::SpotLightMove()
{
	//ボス戦時に全体を赤くする
	if (patern_ == Phase::kGoalPoint) {
		lightcontrol_->FieldLightColorChange();
	}
}
//ダメージを食らったときの処理
void GameScene::DamageProcess()
{
	if (playerhp_ > 0) {
		//ダメージを食らったたとき
		if (oldhp_ > playerhp_) {
			posteffectonflag_ = true;
			damagehitflag_ = true;
			damageefectcolor_.w = 1;
			oldhp_ = playerhp_;
			shake_addvalue_ = 4.5f;
			shakingstartflag_ = true;
		}
		//画面を赤くするフラグが立った時
		if (posteffectonflag_ == true) {
			postefectcolor_.x = 0.7f;
			if (postefectcolor_.x >= 0.7f) {
				posteffectonflag_ = false;
			}
		}
		//画面を赤くするフラグが立っていない時
		if (posteffectonflag_ == false) {
			postefectcolor_.x -= 0.05f;
			if (postefectcolor_.x <= 0) {
				postefectcolor_.x = 0;
			}
		}
	}
	//体力が0になったら
	else if (playerhp_ <= 0) {
		postefectcolor_.x += 0.01f;
		if (postefectcolor_.x >= 2.0f) {
			gamestate_ = CONTINUE;
		}
	}
	//ダメージを食らったとき
	if (damagehitflag_ == true) {
		damageefectcolor_.w -= 0.02f;
		if (damageefectcolor_.w <= 0) {
			damagehitflag_ = false;
		}
	}
}
//1回目の戦闘地点
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
//2回目の戦闘地点
void GameScene::MoveStartFront()
{
	Action::GetInstance()->EaseOut(eyerot_.y, -5.0f);
	if (eyerot_.y <= 0) {
		velocity_ = { 0, 0, 0 };
		gamestate_ = FIGHT;
		stopflag_ = true;
	}
}
//3回目の戦闘地点
void GameScene::MovePointA()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[2] >= 40) {
		velocity_ = { 0.f,0.f,0.f };
		gamestate_ = FIGHT;
		stopflag_ = true;
	}
}
//4回目の戦闘地点
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
//5回目の戦闘地点
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
//6回目の戦闘地点
void GameScene::MovePointC()
{
	velocity_ = { 0, 0, movespeed_ };
	if (cameravector_.m128_f32[0] >= 45) {
		gamestate_ = FIGHT;
		stopflag_ = true;
		velocity_ = { 0, 0, 0 };
	}
}
//7回目の戦闘地点
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
//8回目の戦闘地点
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
//ボスの戦闘地点
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
//ゴール
void GameScene::GoalPoint()
{
	movie_sequence_ = MovieSequence::kAction;
	movie_->Disply();
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
				//ゴールに着いたとき
				if (goalpos_.m128_f32[1] >= 100) {
					gamestate_ = CLEAR;

				}
			}
		}
	}
}
//移動用の関数ポインタ
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
//オブジェクトの描画処理
void GameScene::ObjDraw(DirectXCommon* dxCommon)
{
	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	common_background_->Draw();
	game_background_->Draw();
	goal_->Draw();
	hane_->Draw();
	if (movie_sequence_ == MovieSequence::kAction) {
		heri_->Draw();
	}
	if (movie_sequence_ == MovieSequence::kFinish) {
		player_->ObjDraw();
	}
	player_->ParticleDraw(dxCommon->GetCmdList());
	////オブジェクト後処理
	Object3d::PostDraw();
	//描画
	enemypop_->Draw(dxCommon);
}
//スプライトの描画処理
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
	if ((gamestate_ == FIGHT || gamestate_ == MOVE) && movie_sequence_ == MovieSequence::kFinish) {
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

	ImGui::SliderFloat("eyerot", &passrot_.y, -180.0f, 180.0f);
	ImGui::SliderFloat("eyerotY", &eyerot_.y, -180.0f, 180.0f);

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
	continue_screen_.reset();
	clear_.reset();
	shot_.reset();
	player_.reset();
}