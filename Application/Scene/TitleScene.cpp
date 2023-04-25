#include "TitleScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
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
const XMFLOAT2 SpriteSize = { 64.f,64.f };

//コンストラクタ
TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//初期化処理
void TitleScene::Initialize(DirectXCommon* dxComon)
{
	titlecamera_ = make_unique<Camera>(WinApp::window_width, WinApp::window_height); 
	Object3d::SetCamera(titlecamera_.get());
	light_ = make_unique<Light>();
	light_ = Light::Create();

	lightgroupe_ = make_unique<LightGroup>();
	lightgroupe_ = LightGroup::Create();

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
	for (int i = {}; i < MaxRemainingBullet; i++) {
		bullet_spritepos_[i] = { 1220.0f,25.0f + 32.0f * i };
		bullet_spriterot_[i] = {};
		time_[i] = {};
		bullet_hud_[i].reset(Sprite::SpriteCreate(Name::kBullet, bullet_spritepos_[i], RegularColor, anchorpoint_));
		bullet_hud_[i]->SetSize(SpriteSize);
		drop_bulletflag_[i] = false;
	}

	//オブジェクトの生成
	sphere_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSkydome));
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
		builslowalpha_[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(kBuils));
	}
	start_ = Object3d::Create(ModelManager::GetInstance()->GetModel(8));
	world_ = Object3d::Create(ModelManager::GetInstance()->GetModel(9));
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
	//HUDのポジションセット
	for (int i = 0; i < MaxRemainingBullet; i++) {
		bullet_hud_[i]->SetPosition(bullet_spritepos_[i]);
		bullet_hud_[i]->SetRotation(bullet_spriterot_[i]);
	}
	//カメラの動き
	titlecamera_->MoveEyeVector(cameraeyemove_);
	titlecamera_->MoveVector(cameramove_);
	//天球のステータスのセット
	sphere_->SetRotation(sphererot_);
	sphere_->SetPosition(spherepos_);
	sphere_->SetScale(spherescl_);
	//地面のステータスのセット
	world_->SetPosition(worldpos_);
	world_->SetScale(worldscl_);
	//左右のビルのステータスのセット
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
	//カメラの移動先のビルのステータスセット
	start_->SetPosition(startpos_);
	start_->SetScale(startscl_);
	start_->SetRotation({ 0.0f,180.0f,0.0f });
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
	//左右のビルの更新処理
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i]->Update({ 0.8f,0.6f,0.3f,1.0f });
		builslowalpha_[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}
	//ポストエフェクトの更新処理
	posteffct_->Update(postefectcolor_);
	//天球の更新処理
	sphere_->Update({ 1,1,1,1 },true);
	//地面の更新処理
	world_->Update();
	//カメラの移動先のビルの更新処理
	start_->Update({ 0.4f,0.4f,0.4f,1.f });
	//ライトグループ更新
	lightgroupe_->Update();
}

//更新処理
void TitleScene::Update()
{

	UiEasingProcess();
	
	Mouse::GetInstance()->MouseMoveSprite(reticlepos_);
	//カメラのムーブ関数
	CameraDirection();
	//カーソルがスプライトの範囲内であるか
	CheckCursorIn(reticlepos_, clickpos_, 250.f, 50.f, signalcurorinflag_);
	//最初のクリック
	if (signalcurorinflag_ == true ) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			titlespriteflag_ = false;
			cameraeyemoveflag_ = true;
		}
	}

	DescriptionPageProces();

	FadeOutAndSceneChange();

	StatusSet();

	AllUpdate();
	//カメラの再計算
	titlecamera_->RecalculationMatrix();
}

//カメラの移動
void TitleScene::CameraDirection()
{
	cameramove_ = { 0.0f,0.0f,0.0f };
	if (cameraeyemoveflag_ == true && camerachangeflag_ == false) {
		if (cameraeyemove_.m128_f32[0] >= -1.1) {
			cameraeyemove_.m128_f32[0] -= CameraEyeMoveValue;
			cameramove_.m128_f32[0] += CameraMoveValueXandY;
			cameramove_.m128_f32[1] -= CameraMoveValueXandY;
			cameramove_.m128_f32[2] -= CameraMoveValueZ;
		}
		else {
			camerachangeflag_ = true;
		}
	}

	if (camerachangeflag_ == true) {
		cameraeyemove_ = { 0.0f,0.0f,0.0f };
	}
}

void TitleScene::UiEasingProcess()
{

	if (easingchangeflag_ == false) {
		easingtimer_ += 0.05f;
		if (easingtimer_ >= 1) {
			easingchangeflag_ = true;
		}
	}
	else {
		easingtimer_ -= 0.05f;
		if (easingtimer_ <= -1) {
			easingchangeflag_ = false;
		}
	}
	arrowsize_.x = Action::GetInstance()->EasingOut(easingtimer_, 5) + 32;
	arrowsize_.y = Action::GetInstance()->EasingOut(easingtimer_, 5) + 32;
	clicksize_.x = Action::GetInstance()->EasingOut(easingtimer_, 5) + 550;
	clicksize_.y = Action::GetInstance()->EasingOut(easingtimer_, 5) + 60;
}

//カーソルが範囲内に入っているか
void TitleScene::CheckCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY, bool& CheckFlag)
{
	if ((check_Pos.x - radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) &&
		(check_Pos.y - radY <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		CheckFlag = true;
	}
	else {
		CheckFlag = false;
	}
}

//矢印のスプライトの範囲
bool TitleScene::NextorBack(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY)
{
	if ((check_Pos.x - radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) &&
		(check_Pos.y - radY <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		return true;
	}

	return false;
}


void TitleScene::DescriptionPageProces()
{
	//カメラが移動した後の画面
	if (descriptionpage_ < GameStartPrepartionPage &&
		camerachangeflag_ == true) {
		if (NextorBack(reticlepos_, arrowrightpos_, 16, 16)) {
			arrowrightcolor_ = { 1.f,0.f,0.f,1.f };
			righttrueinflag_ = true;
			//矢印を押された時
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				descriptionpage_ += 1;
			}
		}
		else {
			arrowrightcolor_ = { 1.f,1.f,1.f,1.f };
			righttrueinflag_ = false;
		}
	}

	//ページが0以上であれば
	if (descriptionpage_ > DescriptionPage) {
		if (NextorBack(reticlepos_, arrowleftpos_, 16, 16)) {
			arrowleftcolor_ = { 1.f,0.f,0.f,1.f };
			lefttrueinflag_ = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				clickse_->LoadFile("Resources/Sound/SE/click.wav", volume_);
				descriptionpage_ -= 1;
			}
		}
		else {
			arrowleftcolor_ = { 1.f,1.f,1.f,1.f };
			lefttrueinflag_ = false;
		}
	}

	//救援ヘリを呼ぶとき
	if (camerachangeflag_ == true &&
		signalcurorinflag_ == true &&
		descriptionpage_ == GameStartPrepartionPage) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (clickflag_ == true) {
				clickse_->LoadFile("Resources/Sound/SE/MorseCode.wav", volume_);
				clickflag_ = false;
			}
			fadeoutflag_ = true;
		}
	}
}

//フェードアウト後にゲームシーンへチェンジ
void TitleScene::FadeOutAndSceneChange()
{
	//救援ヘリを読んだ後
	if (fadeoutflag_ == true) {
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
	world_->Draw();
	sphere_->Draw();
	for (int i = 0; i < BUILSAMOUNT; i++) {
		builshighalpha_[i]->Draw();
		builslowalpha_[i]->Draw();
	}
	start_->Draw();
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	if (titlespriteflag_ == true) {
		title_->Draw();
	}
	if (cameraeyemoveflag_ == false) {
		if (signalcurorinflag_ == false) {
			clickbefore_->Draw();
		}
		else if (signalcurorinflag_ == true) {
			clickafter_->Draw();
		}
	}
	else if (camerachangeflag_ == true) {
		if (descriptionpage_ < GameStartPrepartionPage) {
			arrowright_->Draw();
		}
		if (descriptionpage_ > DescriptionPage) {

			arrowleft_->Draw();


		}
		if (descriptionpage_ == DescriptionPage) {
			descriptionoperation_->Draw();
			for (int i = 0; i < MaxRemainingBullet; i++) {
				if (remaining_ <= MaxRemainingBullet) {
					bullet_hud_[i]->Draw();
				}
			}
		}
		else if (descriptionpage_ == EnemyOverViewPage) {
			enemyoverview_->Draw();
		}
		else if (descriptionpage_ == GameStartPrepartionPage) {
			gamestartpreparation_->Draw();
		}
		if (descriptionpage_ == GameStartPrepartionPage) {
			if (signalcurorinflag_ == false) {
				signalbefore_->Draw();
			}
			else if (signalcurorinflag_ == true) {
				signalafter_->Draw();
			}
		}
	}
	cursor_->Draw();
	Sprite::PostDraw();
	posteffct_->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	posteffct_->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
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