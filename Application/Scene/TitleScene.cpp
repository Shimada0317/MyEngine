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
	const XMFLOAT4 RegularColor = { 1.f,1.f,1.f,1.f };
	const XMFLOAT2 DescriptionScreenPosition = { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f };

	//カメラの生成
	titlecamera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(titlecamera_.get());
	//ライトの生成
	light_ = make_unique<Light>();
	light_ = Light::Create();
	lightgroupe_ = make_unique<LightGroup>();
	lightgroupe_ = LightGroup::Create();
	//ライトのセット
	Object3d::SetLightGroup(lightgroupe_.get());
	Object3d::SetLight(light_.get());
	//スプライトの生成
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

	arrowleft_->SetSize(arrowsize_);
	arrowright_->SetSize(arrowsize_);
	arrowleft_->SetColor(arrowleftcolor_);
	arrowright_->SetColor(arrowrightcolor_);

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
	const float CameraMoveValueXandY = 0.4f;

	const float CameraMoveValueZ = 0.1f;

	const float CameraEyeMoveValue = 0.01f;

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
//UIの拡大縮小
void TitleScene::UiEasingProcess()
{
	const int timerlimit_ = 1;
	const int addsize_ = 5;
	const float variable_arrowsize_ = 32;
	const float absolutevalue_ = 0.05f;
	const XMFLOAT2 variable_clicksize_{ 550,60 };

	if (!easingchangeflag_) {
		easingtimer_ += absolutevalue_;
		//タイマーが一定の値になったら反転させる
		if (easingtimer_ >= timerlimit_) {
			easingchangeflag_ = true;
		}
	}
	else {
		easingtimer_ -= absolutevalue_;
		//タイマーが一定の値になったら反転させる
		if (easingtimer_ <= -timerlimit_) {
			easingchangeflag_ = false;
		}
	}
	arrowsize_.x = Action::GetInstance()->EasingOut(easingtimer_, addsize_) + variable_arrowsize_;
	arrowsize_.y = Action::GetInstance()->EasingOut(easingtimer_, addsize_) + variable_arrowsize_;
	clicksize_.x = Action::GetInstance()->EasingOut(easingtimer_, addsize_) + variable_clicksize_.x;
	clicksize_.y = Action::GetInstance()->EasingOut(easingtimer_, addsize_) + variable_clicksize_.y;
	bullet_ui_->ReloadMotion();
}

void TitleScene::ArrowProces()
{
	const XMFLOAT2 rad{ 16.f,16.f };
	const int absolutevalue_ = 1;
	//カメラが移動した後の画面
	if (titlestate_ < GAMESTARTPREPARTIONPAGE &&
		camerachangeflag_) {
		if (Collision::GetInstance()->ChangeAtClick(reticlepos_, arrowrightpos_, rad.x, rad.y)) {
			arrowrightcolor_ = { 1.f,0.f,0.f,1.f };
			righttrueinflag_ = true;
			//矢印を押された時
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				titlestate_ += absolutevalue_;
			}
		}
		else {
			arrowrightcolor_ = { 1.f,1.f,1.f,1.f };
			righttrueinflag_ = false;
		}
	}
	//ページが0以上であれば
	if (titlestate_ > DESCRIPTIONPAGE) {
		if (Collision::GetInstance()->ChangeAtClick(reticlepos_, arrowleftpos_, 16, 16)) {
			arrowleftcolor_ = { 1.f,0.f,0.f,1.f };
			lefttrueinflag_ = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				titlestate_ -= absolutevalue_;
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

//フェードアウト後にゲームシーンへチェンジ
void TitleScene::FadeOutAndSceneChange()
{
	const float SubColor = 0.01f;
	//救援ヘリを読んだ後
	if (fadeoutflag_) {
		postefectcolor_.x -= SubColor;
		postefectcolor_.y -= SubColor;
		postefectcolor_.z -= SubColor;
		postefectcolor_.w -= SubColor;
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
	Object3d::PreDraw(dxCommon->GetCmdList());
	common_background_->Draw();
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

	if (titlestate_ < GAMESTARTPREPARTIONPAGE&&titlestate_>TITLESCREEN) {
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
		//回転の減算する値
		const float subrotation_ = 9.5f;
		//タイマーの加算する値
		const int addtime_ = 1;
		//タイマーを除算するための値
		const int divtime_ = 40;
		int anser_ = 0;
		const int full = 0;
		//残弾を一度非表示にする
		remaining_ = MaxRemainingBullet;
		bullet_ui_->SetRemainig(remaining_);
		//タイムを加算する
		reloadtime_ += addtime_;
		//動かしているタイムを40で除算
		anser_ = reloadtime_ % divtime_;
		//reloaadtime/40の余りが0以外の時
		if (anser_ != 0) { return; }
		//残弾マックスに
		remaining_ = {};
		bullet_ui_->Reload(remaining_);
		//残弾が満タンになった時
		if (remaining_ == full) {
			//タイムを初期化
			reloadtime_ = {};
			hudstate_ = WAIT;
		}
	}
}

void TitleScene::ShotProcess()
{
	const int subbullet_ = 1;
	if (remaining_ < MaxRemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			remaining_ += subbullet_;
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
	//救援ヘリを呼ぶとき
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
	light_.reset();
	bgm_.reset();
	clickse_.reset();
	common_background_.reset();
}
