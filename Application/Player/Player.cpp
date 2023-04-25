#include "Player.h"
#include<cassert>
#include "imgui.h"
#include"imconfig.h"
#include"Action.h"
#include"ModelManager.h"
#include"Mouse.h"
#include"WinApp.h"

#include"SpriteManager.h"

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
	part_green_.reset();
	part_red_.reset();
	part_smoke_.reset();
	shot_se_.reset();
	reload_se_.reset();
}
//初期化処理
void Player::Initalize(Camera* camera)
{
	//HUDのサイズ
	const XMFLOAT2 SpriteSiz = { 64.0f,64.0f };
	//色
	const XMFLOAT4 Color{ 1.f,1.f,1.f,1.f };
	//オブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	//スプライトの読み込み
	sprite_reticle_.reset(Sprite::SpriteCreate(Name::kReticle, reticle_pos2d_, Color, anchorpoint_));
	reload_.reset(Sprite::SpriteCreate(Name::kReload, reload_spritepos_, reload_spritecolor_, anchorpoint_));
	for (int i = {}; i < MaxRemainingBullet; i++) {
		bullet_spritepos_[i] = { 1220.0f,25.0f + 32.0f * i };
		bullet_spriterot_[i] = {};
		time_[i] = {};
		bullet_hud_[i].reset(Sprite::SpriteCreate(Name::kBullet, bullet_spritepos_[i], Color, anchorpoint_));
		bullet_hud_[i]->SetSize(SpriteSiz);
		drop_bulletflag_[i] = false;
	}
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
	XMMATRIX GunNotParentMatrix = gun_->GetNotParentWorld();
	gun_notparentpos_ = XMVector3Transform(gun_pos_, GunNotParentMatrix);
	gun_->SetPosition(gun_pos_);
	//HUDのポジションセット
	for (int i = 0; i < MaxRemainingBullet; i++) {
		bullet_hud_[i]->SetPosition(bullet_spritepos_[i]);
		bullet_hud_[i]->SetRotation(bullet_spriterot_[i]);
	}
	//リロードの文字
	reload_->SetSize(reload_spritesize_);
	reload_->SetColor(reload_spritecolor_);
}
//オブジェクトなどの更新処理
void Player::AllUpdate()
{
	//赤
	const XMFLOAT4 ColorRed{ 1.f,0.f,0.f,0.f };
	//緑
	const XMFLOAT4 ColorGreen{ 0.f,0.5f,0.f,0.f };
	//灰色
	const XMFLOAT4 ColorSmoke{ 0.1f,0.1f,0.1f,0.f };
	//プレイヤー本体の更新
	body_->Update();
	//銃の更新
	gun_->Update();
	//赤いパーティクルの更新処理
	part_red_->Update(ColorRed);
	//緑のパーティクルの更新処理
	part_green_->Update(ColorGreen);
	//煙の更新処理
	part_smoke_->Update(ColorSmoke);
}
//更新処理
void Player::Update(Camera* camera, Phase patern, XMFLOAT3 eyerot)
{
	//マウス操作
	MouseContoroll();
	//待機状態の処理
	WaitProcess();
	//発砲の処理
	GunShotProcess(patern);
	//UI
	HUDMotionProcess();
	//リロードの処理
	ReloadProcess();

	velocity_ = XMVector3TransformNormal(velocity_, body_mat_);

	//座標や回転、スケールなどのステータスのセット
	StatusSet(camera, eyerot);
	//全ての更新処理
	AllUpdate();
}
//待機状態の処理
void Player::WaitProcess()
{
	//ステータスが待機状態の時
	if (player_state_ == WAIT) {
		//除算する値
		const int dividevaluY = 10;
		const int dividevaluX = 50;
		//マウス座標から角度の取得
		gun_rot_.y = (reticle_pos2d_.x - screenhalfwidth_) / dividevaluY;
		gun_rot_.x = (reticle_pos2d_.y - screenhalfheight_) / dividevaluX;
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
		for (int i = 0; i < MaxRemainingBullet; i++) {
			if (remaining_ <= MaxRemainingBullet) {
				bullet_hud_[i]->Draw();
			}
		}
		if (remaining_ >= MaxRemainingBullet) {
			reload_->Draw();
		}
	}
	sprite_reticle_->Draw();
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
	const int addvalue = 15;
	//マウス座標の取得
	Mouse::GetInstance()->MouseMoveSprite(reticle_pos2d_);
	//銃を撃った時
	if (recoil_gunflag_) {
		reticle_pos2d_.y += addvalue;
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
	const int addvalue = 1;
	//弾の発射前
	if (player_state_ == WAIT && remaining_ < MaxRemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			player_state_ = SHOT;
			remaining_ += addvalue;
			recoil_gunflag_ = true;
			ParticleEfect(paterncount);
		}
	}
	//ステータスがSHOTに切り替わった時
	if (player_state_ == SHOT) {
		//弾が発射された
		bullet_shotflag_ = true;
		player_state_ = WAIT;
	}
	//残弾が減った時
	if (old_remaining_ < remaining_) {
		drop_bulletflag_[old_remaining_] = true;
		old_remaining_ = remaining_;
	}
	//リコイル処理
	RecoilProcess();
}

void Player::HUDMotionProcess()
{
	//もし残弾が0になったら
	if (remaining_ > MaxRemainingBullet) {
		//反転フラグがtrueの時
		if (!revers_flag_) {
			//HUDを徐々に大きくする
			SlowlyLargeHUD();
		}
		//反転フラグがfalseのとき
		else {
			//HUDを徐々に小さく
			SlowlySmallHUD();
		}
	}
	//残弾が満タンの時
	else if (remaining_ == 0) {
		for (int i = 0; i < MaxRemainingBullet; i++) {
			bullet_spritepos_[i] = { 1220.0f,25.0f + 32.0f * i };
			bullet_spriterot_[i] = {};
			time_[i] = {};
			drop_bulletflag_[i] = false;
			old_remaining_ = remaining_;
		}
	}
	//落ちていく薬莢の処理
	FallingHUD();
}

void Player::RecoilProcess()
{
	//タイムを加算する値
	const float addrecovery_ = 0.2f;
	//反動の角度
	const int bouncerotation_ = 25;
	//recoverytimeの上限値
	const int timelimit_ = 1;
	//リコイルフラグがtrueの時
	if (recoil_gunflag_) {
		//タイムを加算
		recovery_time_ += addrecovery_;
		//銃の反動で上に向ける
		gun_rot_.x = -bouncerotation_;
		//タイムが上限値まで来たら
		if (recovery_time_ >= timelimit_) {
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
	const int emptyremaining_ = 8;
	//回転の減算する値
	const float subrotation_ = 9.5f;
	//タイマーの加算する値
	const int addtime_ = 1;
	//タイマーを除算するための値
	const int divtime_ = 40;
	int anser_ = 0;
	//ステータスが待機状態で、残弾が満タン以外の時
	if (player_state_ == WAIT && remaining_ != 0) {
		//右クリックした時
		if (Mouse::GetInstance()->PushClick(1)) {
			//ステータスをRELOADに変更
			player_state_ = RELOAD;
			reload_se_->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
			//マウスを操作出来ない状態に

			mouse_stopflag_ = true;
		}
	}
	//ステータスがRELOADではないとき
	if (player_state_ != RELOAD) { return; }
	//銃を回転させる
	gun_rot_.x -= subrotation_;
	//残弾を一度非表示にする
	remaining_ = emptyremaining_;
	//タイムを加算する
	reload_time_ += addtime_;
	//動かしているタイムを40で除算
	anser_ = reload_time_ % divtime_;
	//reloaadtime/40の余りが0以外の時
	if (anser_ != 0) { return; }
	//残弾マックスに
	remaining_ = {};
	//回転していない状態に戻す
	gun_rot_.x = {};
	//残弾が満タンになった時
	if (remaining_ == 0) {
		//ステータスを待機状態に戻す
		player_state_ = WAIT;
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
		XMFLOAT3 pos{};
		float radX = reticle_rot_.y * XM_PI / 180.f;
		float radY = gun_rot_.x * XM_PI / 180.f;
		float sinradX = sinf(radX);
		float cosradX = cosf(radX);

		float sinradY = sinf(radY);
		float cosradY = cosf(radY);
		//後ろを向いているとき
		if (paterncount == LANDINGPOINT_BACK ||
			paterncount == MOVEDPOINT_A ||
			paterncount == MOVEDPOINT_A_LEFT ||
			paterncount == GOALPOINT_BACK) {
			pos.x = gun_worldpos_.m128_f32[0] + sinradX * 3.5f;
			pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gun_worldpos_.m128_f32[2] + 3.0f;
		}
		//右を向いているとき
		else if (paterncount == MOVEDPOINT_C ||
			paterncount == MOVEDPOINT_C_OBLIQUE) {
			pos.x = gun_worldpos_.m128_f32[0] + 2.3f;
			pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
		}
		//真っすぐ前を向いているとき
		else if (paterncount == LANDINGPOINT_FRONT ||
			paterncount == GOALPOINT) {
			pos.x = gun_worldpos_.m128_f32[0] + sinradX * 3.5f;
			pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gun_worldpos_.m128_f32[2] - 3.0f;
		}
		//左を向いているとき
		else if (paterncount == MOVEDPOINT_B) {
			pos.x = gun_worldpos_.m128_f32[0] - 2.3f;
			pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
		}
		//斜めを向いているとき
		else if (paterncount == MOVEDPOINT_C_FRONT) {
			pos.x = gun_worldpos_.m128_f32[0] + 2.3f * sinradX;
			pos.y = gun_worldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gun_worldpos_.m128_f32[2] + 2.8f * cosradX;
		}

		const float rnd_vel = 0.001f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		const float smokernd_vel = 0.05f;
		XMFLOAT3 smokevel{};
		smokevel.x = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
		smokevel.y = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
		smokevel.z = (float)rand() / RAND_MAX * smokernd_vel - smokernd_vel / 2.0f;
		XMFLOAT3 acc{};
		acc.y = 0.0;

		XMFLOAT3 Smokeacc{};
		Smokeacc.y += 0.005f;
		part_red_->Add(20, pos, vel, acc, 0.7f, 0.2f, 1.0f);
		part_green_->Add(20, pos, vel, acc, 0.5f, 0.2f, 1.0f);
		part_smoke_->Add(50, pos, smokevel, acc, 0.5f, 0.0f, 1.0f);
	}
	SoundEffect();

}
void Player::FallingHUD()
{
	//落下時に回転に加算する値
	const float addrotationvalue_ = 80.f;
	//左右に飛ばす値
	const float absolutevalue_ = 10.f;
	//時間に加算する値
	const float addfalltime_ = 0.5f;
	//重力
	const float Gravity = 9.8f;
	//上方向に飛ばす値
	const int upper_ = 40;
	//弾数9発ぶんのfor文
	for (int i = 0; i < MaxRemainingBullet; i++) {
		//落ちるフラグがtrueなら薬莢を下に落とす
		if (drop_bulletflag_[i]) {
			time_[i] += addfalltime_;
			bullet_spritepos_[i].x += Action::GetInstance()->GetRangRand(-absolutevalue_, absolutevalue_);
			Action::GetInstance()->ThrowUp(Gravity, time_[i], upper_, bullet_spritepos_[i].y);
			bullet_spriterot_[i] += addrotationvalue_;
		}
		//落ちたスプライトが画面外に出たらtime_を0にする
		else if (bullet_spritepos_[i].y > WinApp::window_height * 2) {
			time_[i] = {};
		}
	}
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