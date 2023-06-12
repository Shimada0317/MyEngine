#include<cassert>
#include "imgui.h"
#include"imconfig.h"
#include"Action.h"
#include"ModelManager.h"
#include"Mouse.h"
#include"Player.h"
#include"SpriteManager.h"
#include"WinApp.h"
//弾数
const int MaxRemainingBullet = 9;
//縦のスクリーンの半分のサイズ
const float screenhalfheight_ = WinApp::window_height / 2;
//横のスクリーンの半分のサイズ
const float screenhalfwidth_ = WinApp::window_width / 2;
//デストラクタ
Player::~Player()
{
	gun_.reset();
	body_.reset();
	player_ui_.reset();
	shot_se_.reset();
	reload_se_.reset();
}
//初期化処理
void Player::Initalize(Camera* camera)
{
	//色
	const XMFLOAT4 kColor{ 1.f,1.f,1.f,1.f };
	//オブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	//スプライトの読み込み
	sprite_reticle_.reset(Sprite::SpriteCreate(Name::kReticle, reticle_pos2d_, kColor, anchorpoint_));
	bullet_ui_ = make_unique<BulletUI>();
	bullet_ui_->Create(remaining_, ui_bulletpos, ui_reloadpos_);


	player_ui_ = make_unique<PlayerUI>();
	player_ui_->Create();
	//オブジェクトの読み込み
	gun_ = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	body_ = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	body_->SetParent(camera);
	//パーティクルの生成
	part_red_.reset(ParticleManager::Create(camera));
	part_green_.reset(ParticleManager::Create(camera));
	part_smoke_.reset(ParticleManager::Create(camera));
	//音の生成
	shot_se_ = make_unique<Audio>();
	reload_se_ = make_unique<Audio>();
	shot_se_->Initialize();
	reload_se_->Initialize();

};
//ステータスセット
void Player::StatusSet(Camera* camera, XMFLOAT3 eyerot)
{
	//本体のワールド座標取得
	body_mat_ = body_->GetMatrix();
	body_worldpos_ = { -10.0f,0.0f,-20.0f };
	body_worldpos_ = XMVector3Transform(body_worldpos_, body_mat_);
	//レティクルの位置から角度の算出
	change_rot_ = eyerot;
	reticle_rot_.y = (reticle_pos2d_.x - screenhalfwidth_) / 10 + change_rot_.y;
	//本体のパーツのステータスセット
	body_->SetRotation(body_rot_);
	body_->SetPosition(body_pos_);
	body_->SetScale(body_scl_);
	body_->SetParent(camera);
	//銃のワールド座標取得とステータスセット
	gun_mat_ = gun_->GetMatrix();
	gun_worldpos_ = XMVector3Transform(gun_pos_, gun_mat_);
	gun_->SetRotation(gun_rot_);
	gun_->SetScale(gun_scl_);
	gun_->SetParent(camera);
	XMMATRIX kGetGunNotParentMatrix = gun_->GetNotParentWorld();
	gun_notparentpos_ = XMVector3Transform(gun_pos_, kGetGunNotParentMatrix);
	gun_->SetPosition(gun_pos_);
	//UIのポジションセット
	bullet_ui_->Set();
}
//オブジェクトなどの更新処理
void Player::AllUpdate()
{
	//赤
	const XMFLOAT4 kColorRed{ 1.f,0.f,0.f,0.f };
	//緑
	const XMFLOAT4 kColorGreen{ 0.f,0.5f,0.f,0.f };
	//灰色
	const XMFLOAT4 kColorSmoke{ 0.1f,0.1f,0.1f,0.f };
	//プレイヤー本体の更新
	body_->Update();
	//銃の更新
	gun_->Update();
	//赤いパーティクルの更新処理
	part_red_->Update(kColorRed);
	//緑のパーティクルの更新処理
	part_green_->Update(kColorGreen);
	//煙の更新処理
	part_smoke_->Update(kColorSmoke);
}
//更新処理
void Player::Update(Camera* camera, Phase patern, XMFLOAT3 eyerot, int gamestate, int state)
{
	//マウス操作
	MouseContoroll();
	//待機状態の処理
	WaitProcess();
	//座標や回転、スケールなどのステータスのセット
	StatusSet(camera, eyerot);
	//全ての更新処理
	AllUpdate();
	if (gamestate == state) { return; }
	//発砲の処理
	GunShotProcess(patern);
	//UI
	UIMotionProcess();
	//リロードの処理
	ReloadProcess();
}
//待機状態の処理
void Player::WaitProcess()
{
	//ステータスが待機状態の時
	if (player_state_ == State::kWait) {
		//除算する値
		const int kDivideValuY = 10;
		const int kDivideValuX = 50;
		//マウス座標から角度の取得
		gun_rot_.y = (reticle_pos2d_.x - screenhalfwidth_) / kDivideValuY;
		gun_rot_.x = (reticle_pos2d_.y - screenhalfheight_) / kDivideValuX;
		bullet_shotflag_ = false;
	}
}
//パーティクル描画
void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	//マズルフラッシュの描画
	ParticleManager::PreDraw(cmdeList);
	part_smoke_->Draw();
	part_red_->Draw();
	part_green_->Draw();
	ParticleManager::PostDraw();
}
//スプライト描画
void Player::SpriteDraw()
{
	if (!mouse_stopflag_) {
		bullet_ui_->Draw();
	}
	sprite_reticle_->Draw();
	player_ui_->Draw(hp_);
}
//オブジェクト描画
void Player::ObjDraw()
{
	//Hpが0以上であれば
	if (hp_ >= 0) {
		gun_->Draw();
	}
}

void Player::SoundEffect()
{
	shot_se_->LoadFile("Resources/Sound/SE/shot.wav", 0.3f);
}
//マウス操作
void Player::MouseContoroll()
{
	//加算する値
	const int kAddValue = 15;
	//マウス座標の取得
	Mouse::GetInstance()->MouseMoveSprite(reticle_pos2d_);
	//銃を撃った時
	if (recoil_gunflag_) {
		reticle_pos2d_.y += kAddValue;
		Mouse::GetInstance()->RecoilMouse(reticle_pos2d_);
	}
	else {
		//取得した座標をレティクルにセット
		sprite_reticle_->SetPosition(reticle_pos2d_);
	}
}

//弾の発射処理
void Player::GunShotProcess(Phase paterncount)
{
	//加算する値
	const int kAddValue = 1;
	//弾の発射前
	if (player_state_ == State::kWait && remaining_ < MaxRemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			remaining_ += kAddValue;
			bullet_ui_->Shot(remaining_);
			recoil_gunflag_ = true;
			player_state_ = State::kShot;
			//マズルフラッシュ
			ParticleEfect(paterncount);
		}
	}
	//ステータスがSHOTに切り替わった時
	if (player_state_ == State::kShot) {
		//弾が発射された
		bullet_shotflag_ = true;
		player_state_ = State::kWait;
	}
	//リコイル処理
	RecoilProcess();
}

void Player::UIMotionProcess()
{
	//UIの鼓動
	player_ui_->HartBeat(hp_);
	//リロード
	bullet_ui_->ReloadMotion();
	//落ちていく薬莢の処理
	bullet_ui_->FallingUI();
}

void Player::RecoilProcess()
{
	//タイムを加算する値
	const float kAddRecovery = 0.2f;
	//反動の角度
	const int kBounceRotation = 25;
	//recoverytimeの上限値
	const int kTimeLimit = 1;
	//リコイルフラグがtrueの時
	if (recoil_gunflag_) {
		//タイムを加算
		recovery_time_ += kAddRecovery;
		//銃の反動で上に向ける
		gun_rot_.x = -kBounceRotation;
		//タイムが上限値まで来たら
		if (recovery_time_ >= kTimeLimit) {
			//反動を元に戻す
			gun_rot_.x = {};
			//タイムを初期化
			recovery_time_ = {};
			//フラグをfalseに戻す
			recoil_gunflag_ = false;
		}
	}
}

//リロード処理
void Player::ReloadProcess()
{
	//残弾が空
	const int kEmptyRemaining = 8;
	//回転の減算する値
	const float kSubRotation = 9.5f;
	//タイマーの加算する値
	const int kAddTime = 1;
	//タイマーを除算するための値
	const int kDivTime = 40;
	int kAnser = 0;
	//ステータスが待機状態で、残弾が満タン以外の時
	if (player_state_ == State::kWait && remaining_ != 0) {
		//右クリックした時
		if (Mouse::GetInstance()->PushClick(1)) {
			//ステータスをRELOADに変更
			player_state_ = State::kReload;
			reload_se_->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
			//マウスを操作出来ない状態に

			mouse_stopflag_ = true;
		}
	}
	//ステータスがRELOADではないとき
	if (player_state_ != State::kReload) { return; }
	//銃を回転させる
	gun_rot_.x -= kSubRotation;
	//残弾を一度非表示にする
	remaining_ = kEmptyRemaining;
	bullet_ui_->SetRemainig(remaining_);
	//タイムを加算する
	reload_time_ += kAddTime;
	//動かしているタイムを40で除算
	kAnser = reload_time_ % kDivTime;
	//reloaadtime/40の余りが0以外の時
	if (kAnser != 0) { return; }
	//残弾マックスに
	remaining_ = {};
	bullet_ui_->Reload(remaining_);
	//残弾が満タンになった時
	if (remaining_ == 0) {
		//ステータスを待機状態に戻す
		player_state_ = State::kWait;
		//タイムを初期化
		reload_time_ = {};
		//操作を再度可能状態にする
		mouse_stopflag_ = false;
	}
}


//マズルエフェクト
void Player::ParticleEfect(Phase paterncount)
{
		for (int i = 0; i < 10; i++) {
			float radX = reticle_rot_.y * XM_PI / 180.f;
			float radY = gun_rot_.x * XM_PI / 180.f;
			float sinradX = sinf(radX);
			float cosradX = cosf(radX);
			float sinradY = sinf(radY);
			float cosradY = cosf(radY);
			//後ろを向いているとき
			if (paterncount == Phase::kLandingPointBack ||
				paterncount == Phase::kMovedPointA ||
				paterncount == Phase::kMovedPointALeft ||
				paterncount == Phase::kGoalPointBack) {
				pos.x = gun_worldpos_.m128_f32[0] + sinradX * 3.5f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 3.0f;
			}
			//右を向いているとき
			else if (paterncount == Phase::kMovedPointC ||
				paterncount == Phase::kMovedPointCOblique) {
				pos.x = gun_worldpos_.m128_f32[0] + 2.3f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
			}
			//真っすぐ前を向いているとき
			else if (paterncount == Phase::kLandingPointFront ||
				paterncount == Phase::kGoalPoint) {
				pos.x = gun_worldpos_.m128_f32[0] + sinradX * 3.5f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] - 3.0f;
			}
			//左を向いているとき
			else if (paterncount == Phase::kMovedPointB) {
				pos.x = gun_worldpos_.m128_f32[0] - 2.3f;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
			}
			//斜めを向いているとき
			else if (paterncount == Phase::kMovedPointCFront) {
				pos.x = gun_worldpos_.m128_f32[0] + 2.3f * sinradX;
				pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
				pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
			}

			const float rnd_vel = 0.001f;
			XMFLOAT3 vel{};
			vel = Action::GetInstance()->RandMax(vel,rnd_vel);

			const float smokernd_vel = 0.05f;
			XMFLOAT3 smokevel{};
			smokevel = Action::GetInstance()->RandMax(smokevel, smokernd_vel);
			
			XMFLOAT3 acc{};
			acc.y = 0.0;

			XMFLOAT3 Smokeacc{};
			Smokeacc.y += 0.005f;
			part_red_->Add(20, pos, vel, acc, 0.7f, 0.f, 1.f);
			part_green_->Add(20, pos, vel, acc, 0.5f, 0.f, 1.f);
			part_smoke_->Add(50, pos, smokevel, acc, 0.5f, 0.f, 1.f);
		}
		SoundEffect();
}

void Player::SlowlyLargeHUD()
{
	//Reloadの文字を徐々に大きくする
	Action::GetInstance()->EaseOut(reload_spritesize_.x, 260);
	Action::GetInstance()->EaseOut(reload_spritesize_.y, 190);
	if (reload_spritesize_.x >= 250) {
		revers_flag_ = true;
	}
}
void Player::SlowlySmallHUD()
{
	//Reloadの文字を徐々に小さくする
	Action::GetInstance()->EaseOut(reload_spritesize_.x, 200);
	Action::GetInstance()->EaseOut(reload_spritesize_.y, 130);
	if (reload_spritesize_.x <= 210) {
		revers_flag_ = false;
	}
}
//ImgUi描画
void Player::ImGuiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");
	if (ImGui::TreeNode("change")) {
		ImGui::SliderFloat("changerot.x", &change_rot_.x, -100.0f, 100.0f);
		ImGui::SliderFloat("changerot.y", &change_rot_.y, -100.0f, 100.0f);
		ImGui::SliderFloat("changerot.z", &change_rot_.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}