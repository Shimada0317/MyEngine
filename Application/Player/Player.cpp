#include "Player.h"
#include<cassert>
#include "imgui.h"
#include"imconfig.h"
#include"Action.h"
#include"ModelManager.h"
#include"Mouse.h"
#include"WinApp.h"

#include"SpriteManager.h"

const int MaxRemainingBullet = 9;
const float Gravity = 9.8f;
const 	XMFLOAT2 SpriteSiz = { 64.0f,64.0f };
const XMFLOAT4 Color{ 1.f,1.f,1.f,1.f };
const float addspriteposition = 10;
const XMFLOAT4 ColorRed{ 1.f,0.f,0.f,0.f };
const XMFLOAT4 ColorGreen{ 0.f,0.5f,0.f,0.f };
const XMFLOAT4 ColorSmoke{ 0.1f,0.1f,0.1f,0.f };
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

	for (int i = 0; i < MaxRemainingBullet; i++) {
		//落ちるフラグがtrueなら薬莢を下に落とす
		if (drop_bulletflag_[i]) {
			time_[i] += 0.5f;
			bullet_spritepos_[i].x += Action::GetInstance()->GetRangRand(-10, 10);
			Action::GetInstance()->ThrowUp(Gravity, time_[i], 40, bullet_spritepos_[i].y);
			bullet_spriterot_[i] += 80;
		}
		//落ちたスプライトが画面外に出たらtime_を0にする
		else if (bullet_spritepos_[i].y > WinApp::window_height * 2) {
			time_[i] = {};
		}
	}
	
	//本体のワールド座標取得
	body_mat_ = body_->GetMatrix();
	body_worldpos_ = { -10.0f,0.0f,-20.0f };
	body_worldpos_ = XMVector3Transform(body_worldpos_, body_mat_);
	//レティクルの位置から角度の算出
	change_rot_ = eyerot;
	reticle_rot_.y = (reticle_pos2d_.x - WinApp::window_width / 2) / 10 + change_rot_.y;
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
		bullet_hud_[i]->SetSize(SpriteSiz);
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
	body_->Update();
	gun_->Update();
	part_red_->Update(ColorRed);
	part_green_->Update(ColorGreen);
	part_smoke_->Update(ColorSmoke);
}
//更新処理
void Player::Update(Camera* camera, Phase patern, XMFLOAT3 eyerot)
{

	MouseContoroll();

	WaitProcess();

	GunShotProcess(patern);

	UIMotionProcess();

	ReloadProcess();

	velocity_ = XMVector3TransformNormal(velocity_, body_mat_);

	StatusSet(camera, eyerot);

	AllUpdate();
}
//待機状態の処理
void Player::WaitProcess()
{
	//ステータスが待機状態の時
	if (player_state_ == WAIT) {
		gun_rot_.x = (reticle_pos2d_.y - WinApp::window_height / 2) / 50;
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
	//除算する値
	const int dividevalu = 10;
	//マウス座標から角度の取得
	gun_rot_.y = (reticle_pos2d_.x - WinApp::window_width / 2) / dividevalu;
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

void Player::UIMotionProcess()
{
	//もし残弾が0になったら
	if (remaining_ > MaxRemainingBullet) {
		//反転フラグがtrueの時
		if (!revers_flag_) {
			SlowlyLargeHUD();
		}
		//反転フラグがfalseのとき
		else {
			SlowlySmallHUD();
		}
	}
	//残弾が満タンの時
	else if (remaining_ == 0) {
		for (int i = 0; i < MaxRemainingBullet; i++) {
			bullet_spritepos_[i] = { 1220.0f,25.0f + 32.0f * i };
			bullet_spriterot_[i] = 0;
			time_[i] = 0;
			drop_bulletflag_[i] = false;
			old_remaining_ = remaining_;
		}
	}
}

void Player::RecoilProcess()
{
	const float addrecovery_ = 0.2f;
	const int bouncerotation_ = 25;
	//後ろに後退した値
	const float recession = 3.f;
	//recoverytimeの上限値
	const int timelimit_ = 1;
	//リコイルフラグがtrueの時
	if (recoil_gunflag_) {
		recovery_time_ += addrecovery_;
		gun_rot_.x = -bouncerotation_;
		gun_pos_.m128_f32[2] = -recession;
		if (recovery_time_ >= timelimit_) {
			gun_rot_.x = {};
			gun_pos_.m128_f32[2] = -recession;
			recovery_time_ = {};
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
			mouse_stopflag_ = true;
		}
	}
	//ステータスがRELOADの時
	if (player_state_ == RELOAD) {
		gun_rot_.x -= subrotation_;
		remaining_ = emptyremaining_;
		reload_time_ += addtime_;
		anser_ = reload_time_ % divtime_;
		//reloaadtime/40の余りが0の時
		if (anser_ == 0) {
			remaining_ = {};
			gun_rot_.x = {};
			//残弾が満タンになった時
			if (remaining_ == 0) {
				player_state_ = WAIT;
				reload_time_ = {};
				mouse_stopflag_ = false;
			}
		}
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