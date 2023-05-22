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

//コンストラクタ
TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}
//初期化処理
void TitleScene::Initialize(DirectXCommon* dxComon)
{
	const XMFLOAT4 kRegularColor = { 1.f,1.f,1.f,1.f };
	const XMFLOAT2 kDescriptionScreenPosition = { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f };

	//カメラの生成
	titlecamera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(titlecamera_.get());
	//ライトの生成
	lightgroupe_ = make_unique<LightGroup>();
	lightgroupe_ = LightGroup::Create();
	//ライトのセット
	Object3d::SetLightGroup(lightgroupe_.get());
	//スプライトの生成
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
	//オブジェクトの生成
	common_background_ = make_unique<CommonBackground>();
	common_background_->Initialize();
	//SEの初期か
	clickse_ = make_unique<Audio>();
	clickse_->Initialize();
	bgm_ = make_unique<Audio>();
	bgm_->Initialize();
	bgm_->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);
	//ポストエフェクトの初期化
	posteffct_ = make_unique<PostEffect>();
	posteffct_->Initialize();
	cameraeyemove_ = { 0.0f,0.0f,0.0f };
	//スポットライトをアクティブ状態
	lightgroupe_->SetSpotLightActive(0, true);
	lightgroupe_->SetSpotLightActive(1, true);
	lightgroupe_->SetSpotLightActive(2, true);
}
//ステータスセット
void TitleScene::StatusSet()
{
	//弾のUI
	bullet_ui_->Set();
	//カメラの動き
	titlecamera_->MoveEyeVector(cameraeyemove_);
	titlecamera_->MoveVector(cameramove_);
	//タイトルスプライトのステータスセット
	title_->SetSize({ titlesize_ });
	title_->SetPosition({ titlepos_ });
	//マウスカーソルの座標セット
	cursor_->SetPosition({ reticlepos_ });
	//1つ目のスポットライトを設定
	lightgroupe_->SetSpotLightDir(0, XMVECTOR({ spotlightdir_.x, spotlightdir_.y, spotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(0, spotlightpos_);
	lightgroupe_->SetSpotLightColor(0, spotlightcolor_);
	lightgroupe_->SetSpotLightAtten(0, spotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(0, spotlightfactorangle_);
	//2つ目のスポットライトを設定
	lightgroupe_->SetSpotLightDir(1, XMVECTOR({ spotlightdir2_.x, spotlightdir2_.y, spotlightdir2_.z }));
	lightgroupe_->SetSpotLightPos(1, spotlightpos2_);
	lightgroupe_->SetSpotLightColor(1, spotlightcolor2_);
	lightgroupe_->SetSpotLightAtten(1, spotlightatten2_);
	lightgroupe_->SetSpotLightFactorAngle(1, spotlightfactorangle2_);
	//1つ目のスポットライトを設定
	lightgroupe_->SetSpotLightDir(2, XMVECTOR({ spotlightdir_.x, spotlightdir_.y, spotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(2, spotlightpos_);
	lightgroupe_->SetSpotLightColor(2, spotlightcolor_);
	lightgroupe_->SetSpotLightAtten(2, spotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(2, spotlightfactorangle_);
	//矢印のステータス
	arrowleft_->SetSize(arrowsize_);
	arrowright_->SetSize(arrowsize_);
	arrowleft_->SetColor(arrowleftcolor_);
	arrowright_->SetColor(arrowrightcolor_);
	//クリックのステータス
	clickafter_->SetSize(clicksize_);
	clickbefore_->SetSize(clicksize_);
	signalafter_->SetSize(clicksize_);
	signalbefore_->SetSize(clicksize_);
}
//全ての更新処理をまとめる
void TitleScene::AllUpdate()
{
	//ポストエフェクトの更新処理
	posteffct_->Update(postefectcolor_);
	//ライトグループ更新
	lightgroupe_->Update();
	//背景オブジェクトの更新
	common_background_->Update();
}
//更新処理
void TitleScene::Update()
{
	//マウスの座標にスプライトを合わせる
	Mouse::GetInstance()->MouseMoveSprite(reticlepos_);
	//UIをイージングで拡大縮小させる処理
	UiEasingProcess();
	//カメラのムーブ関数
	CameraDirection();
	//操作説明のページ
	DescriptioPageProcess();
	//ゲーム開始のページ
	GameStartPrepartionPage();
	//カーソルがスプライトの範囲内であるか
	Collision::GetInstance()->ToggleFlagInClick(reticlepos_, clickpos_, 250.f, 50.f, signalcurorinflag_);
	//最初のクリック
	if (signalcurorinflag_) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			titlespriteflag_ = false;
			cameraeyemoveflag_ = true;
		}
	}
	// 説明画面で行われる処理
	ArrowProces();
	//フェードアウト後にシーンチェンジ
	FadeOutAndSceneChange();
	//座標、回転、スケールのステータスをセット
	StatusSet();
	//全ての更新処理
	AllUpdate();
	//カメラの再計算
	titlecamera_->RecalculationMatrix();
}
//カメラの移動
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
//UIの拡大縮小
void TitleScene::UiEasingProcess()
{
	const int kTimerLimit = 1;
	const int kAddSize = 5;
	const float kVariableArrowSize = 32;
	const float kAbsoluteValue = 0.05f;
	const XMFLOAT2 kVariableClickSize{ 550,60 };
	if (!easingchangeflag_) {
		easingtimer_ += kAbsoluteValue;
		//タイマーが一定の値になったら反転させる
		if (easingtimer_ >= kTimerLimit) {
			easingchangeflag_ = true;
		}
	}
	else {
		easingtimer_ -= kAbsoluteValue;
		//タイマーが一定の値になったら反転させる
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
// 説明画面で行われる処理
void TitleScene::ArrowProces()
{
	const XMFLOAT2 kRad{ 16.f,16.f };
	const int kAbsoluteValue = 1;
	//カメラが移動した後の画面
	if (titlestate_ < TITLESTATE::kGameStartPreoartionPage &&
		camerachangeflag_) {
		if (Collision::GetInstance()->ChangeAtClick(reticlepos_, arrowrightpos_, kRad.x, kRad.y)) {
			arrowrightcolor_ = { 1.f,0.f,0.f,1.f };
			righttrueinflag_ = true;
			//矢印を押された時
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
	//ページが0以上であれば
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
//フェードアウト後にゲームシーンへチェンジ
void TitleScene::FadeOutAndSceneChange()
{
	const float kSubColor = 0.01f;
	//救援ヘリを読んだ後
	if (fadeoutflag_) {
		Action::GetInstance()->ColorDown(postefectcolor_, kSubColor);
		if (postefectcolor_.w <= 0) {
			//シーン切り替え
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
}
//描画処理
void TitleScene::Draw(DirectXCommon* dxCommon)
{
	posteffct_->PreDrawScene(dxCommon->GetCmdList());
	//オブジェクトの描画
	Object3d::PreDraw(dxCommon->GetCmdList());
	common_background_->Draw();
	Object3d::PostDraw();
	//スプライトの描画
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
// リロード処理
void TitleScene::ReloadProcess()
{
	if (Mouse::GetInstance()->PushClick(1)) {
		hudstate_ = UISTATE::kReload;
	}
	if (hudstate_ != UISTATE::kReload) { return; }
	if (remaining_ != 0) {
		//回転の減算する値
		const float kSubRotation = 9.5f;
		//タイマーの加算する値
		const int kAddTime = 1;
		//タイマーを除算するための値
		const int kDivTime = 40;
		int kAnser = 0;
		const int kFull = 0;
		//残弾を一度非表示にする
		remaining_ = MaxRemainingBullet;
		bullet_ui_->SetRemainig(remaining_);
		//タイムを加算する
		reloadtime_ += kAddTime;
		//動かしているタイムを40で除算
		kAnser = reloadtime_ % kDivTime;
		//reloaadtime/40の余りが0以外の時
		if (kAnser != 0) { return; }
		//残弾マックスに
		remaining_ = {};
		bullet_ui_->Reload(remaining_);
		//残弾が満タンになった時
		if (remaining_ == kFull) {
			//タイムを初期化
			reloadtime_ = {};
			hudstate_ = UISTATE::kWait;
		}
	}
}
// 発砲の処理
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
// 説明のページの処理
void TitleScene::DescriptioPageProcess()
{
	if (titlestate_ != TITLESTATE::kDescriptionPage) { return; }

	ReloadProcess();

	ShotProcess();

	bullet_ui_->FallingUI();
}
// ゲーム開始のページの処理
void TitleScene::GameStartPrepartionPage()
{
	if (titlestate_ != TITLESTATE::kGameStartPreoartionPage) { return; }
	//救援ヘリを呼ぶとき
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

//終了処理
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
