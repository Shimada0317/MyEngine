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
const int ten = 10;

//デストラクタ
Player::~Player()
{

	gun_.reset();
	body_.reset();
	partgreen_.reset();
	partred_.reset();
	shotse_.reset();
	reloadse_.reset();
}

//初期化処理
void Player::Initalize(Camera* camera)
{
	//オブジェクトにカメラをセット
	Object3d::SetCamera(camera);
	//スプライトの読み込み
	spritereticle_.reset(Sprite::SpriteCreate(Name::kReticle, reticlepos2d_, reticlecolor_, reticleancorpoint_));
	curtainup_.reset(Sprite::SpriteCreate(Name::kCurtain, curtainuppos_));
	curtaindown_.reset(Sprite::SpriteCreate(Name::kCurtain, curtaindownpos_));
	curtainup_->SetSize(curtainsize_);
	curtaindown_->SetSize(curtainsize_);
	skip_.reset(Sprite::SpriteCreate(Name::kSkip, skippos_));
	reload_.reset(Sprite::SpriteCreate(Name::kReload, reloadspritepos_, reloadspritecolor_, anchorpoint_));
	for (int i = {}; i < MaxRemainingBullet; i++) {
		spritepos_[i] = { 1220.0f,25.0f + 32.0f * i };
		spriterot_[i] = {};
		time_[i] = {};
		bulletHUD[i].reset(Sprite::SpriteCreate(Name::kBullet, spritepos_[i], Color, anchorpoint_));
		dropbulletflag_[i] = false;
	}
	//オブジェクトの読み込み
	gun_ = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	body_ = Object3d::Create(ModelManager::GetInstance()->GetModel(10));
	body_->SetParent(camera);
	//パーティクルの生成
	partred_.reset(ParticleManager::Create(camera));
	partgreen_.reset(ParticleManager::Create(camera));
	partsmoke_.reset(ParticleManager::Create(camera));
	//音の生成
	shotse_ = make_unique<Audio>();
	reloadse_ = make_unique<Audio>();
	shotse_->Initialize();
	reloadse_->Initialize();
};

//ステータスセット
void Player::StatusSet(Camera* camera)
{

	for (int i = 0; i < MaxRemainingBullet; i++) {
		if (dropbulletflag_[i] == true) {
			spritepos_[i].y += 10;
			time_[i] += 0.5f;
			spritepos_[i].x += Action::GetInstance()->GetRangRand(-10, 10);
			Action::GetInstance()->ThrowUp(Gravity, time_[i], 40, spritepos_[i].y);
			spriterot_[i] += 80;
		}
		else if (spritepos_[i].y > WinApp::window_height * 2) {
			time_[i] = {};
		}
	}

	if (oldremaining_ < remaining_) {
		dropbulletflag_[oldremaining_] = true;
		oldremaining_ = remaining_;
	}

	bodymat_ = body_->GetMatrix();
	bodyworldpos_ = { -10.0f,0.0f,-20.0f };
	bodyworldpos_ = XMVector3Transform(bodyworldpos_, bodymat_);

	reticlerot_.y = (reticlepos2d_.x - WinApp::window_width / 2) / 10 + changerot_;

	body_->SetRotation(bodyrot_);
	body_->SetPosition(bodypos_);
	body_->SetScale(bodyscl_);
	body_->SetParent(camera);

	gunmat_ = gun_->GetMatrix();
	gunworldpos_ = XMVector3Transform(gunpos_, gunmat_);

	gun_->SetRotation(gunrot_);
	gun_->SetScale(gunscl_);
	gun_->SetParent(camera);
	XMMATRIX GunNotParentMatrix = gun_->GetNotParentWorld();

	gunnotparentpos_ = XMVector3Transform(gunpos_, GunNotParentMatrix);
	gun_->SetPosition(gunpos_);
	//HUDのポジションセット
	for (int i = 0; i < MaxRemainingBullet; i++) {
		bulletHUD[i]->SetSize({ SpriteSiz });
		bulletHUD[i]->SetPosition(spritepos_[i]);
		bulletHUD[i]->SetRotation(spriterot_[i]);
	}
	//リロードの文字
	reload_->SetSize(reloadspritesize_);
	reload_->SetColor(reloadspritecolor_);
}


void Player::AllUpdate()
{
	body_->Update();
	gun_->Update();
	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });
	partsmoke_->Update({ 0.1f,0.1f,0.1f,0.0f });
}


//更新処理
void Player::Update(Camera* camera, Phase patern)
{
	DamageProcess();

	MouseContoroll();

	WaitProcess();

	GunShotProcess(patern);

	UIMotionProcess();

	ReloadProcess();

	velocity_ = XMVector3TransformNormal(velocity_, bodymat_);

	StatusSet(camera);

	AllUpdate();
}

void Player::WaitProcess()
{
	if (playerstate_ == WAIT) {
		gunrot_.x = (reticlepos2d_.y - WinApp::window_height / 2) / 50;
	}
}

//パーティクル描画
void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	ParticleManager::PreDraw(cmdeList);
	partsmoke_->Draw();
	partred_->Draw();
	partgreen_->Draw();
	ParticleManager::PostDraw();
}

//スプライト描画
void Player::SpriteDraw()
{
	if (mousestopflag_ == false) {
		for (int i = 0; i < MaxRemainingBullet; i++) {
			if (remaining_ <= MaxRemainingBullet) {
				bulletHUD[i]->Draw();
			}
		}
		if (remaining_ >= MaxRemainingBullet) {
			reload_->Draw();
		}
	}
	spritereticle_->Draw();
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
	shotse_->LoadFile("Resources/Sound/SE/shot.wav", 0.3f);
}

//マウス操作
void Player::MouseContoroll()
{
	//マウス座標から角度の取得
	gunrot_.y = (reticlepos2d_.x - WinApp::window_width / 2) / 10;
	//マウス座標の取得
	Mouse::GetInstance()->MouseMoveSprite(reticlepos2d_);
	//銃を撃った時
	if (recoilgunflag_ == true) {
		reticlepos2d_.y += 15;
		Mouse::GetInstance()->RecoilMouse(reticlepos2d_);
	}
	else {
		//取得した座標をレティクルにセット
		spritereticle_->SetPosition(reticlepos2d_);
	}
}

//ダメージを受けたときの処理
void Player::DamageProcess()
{
	if (oldhp_ > hp_) {
		shakingstartflag_ = true;
		shakingscreenvalue_ = 3.5f;
		oldhp_ = hp_;
	}
}

//弾の発射処理
void Player::GunShotProcess(Phase paterncount)
{
	//弾の発射前
	if (playerstate_ == WAIT && remaining_ < MaxRemainingBullet) {
		if (Mouse::GetInstance()->PushClick(0)) {
			playerstate_ = SHOT;
			remaining_ += 1;
			recoilgunflag_ = true;
			ParticleEfect(paterncount);
		}
	}
	if (playerstate_ == SHOT) {
		//弾が発射された
		bulletshotflag_ = true;
		playerstate_ = WAIT;
	}
	else {
		bulletshotflag_ = false;
	}
	RecoilProcess();
}

void Player::UIMotionProcess()
{
	if (remaining_ > MaxRemainingBullet) {
		if (reversflag_ == false) {
			Action::GetInstance()->EaseOut(reloadspritesize_.x, 260);
			Action::GetInstance()->EaseOut(reloadspritesize_.y, 190);
			if (reloadspritesize_.x >= 250) {
				reversflag_ = true;
			}
		}
		else {
			Action::GetInstance()->EaseOut(reloadspritesize_.x, 200);
			Action::GetInstance()->EaseOut(reloadspritesize_.y, 130);
			if (reloadspritesize_.x <= 210) {
				reversflag_ = false;
			}
		}
	}
	else if (remaining_ == 0) {
		for (int i = 0; i < MaxRemainingBullet; i++) {
			spritepos_[i] = { 1220.0f,25.0f + 32.0f * i };
			spriterot_[i] = 0;
			time_[i] = 0;
			dropbulletflag_[i] = false;
			oldremaining_ = remaining_;
		}
	}
}

void Player::RecoilProcess()
{
	if (recoilgunflag_ == true) {
		recoverytime_ += 0.2f;
		gunrot_.x = -25;
		gunpos_.m128_f32[2] = -3.1f;
		if (recoverytime_ >= 1) {
			gunrot_.x = 0;
			gunpos_.m128_f32[2] = -3.0f;
			recoverytime_ = 0.0f;
			recoilgunflag_ = false;
		}
	}
}

//リロード処理
void Player::ReloadProcess()
{
	if (playerstate_ == WAIT && remaining_ != 0) {
		if (Mouse::GetInstance()->PushClick(1)) {
			playerstate_ = RELOAD;
			reloadse_->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
			mousestopflag_ = true;
		}
	}

	if (playerstate_ == RELOAD) {
		gunrot_.x -= 9.5f;
		remaining_ = 8;
		reloadtime_ += 1;
		anser_ = reloadtime_ % 40;
		if (anser_ == 0) {
			remaining_ = {};
			gunrot_.x = {};
			if (remaining_ == 0) {
				playerstate_ = WAIT;
				reloadtime_ = {};
				mousestopflag_ = false;
			}
		}
	}
}


//マズルエフェクト
void Player::ParticleEfect(Phase paterncount)
{

	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos{};
		float radX = reticlerot_.y * XM_PI / 180.f;
		float radY = gunrot_.x * XM_PI / 180.f;
		float sinradX = sinf(radX);
		float cosradX = cosf(radX);

		float sinradY = sinf(radY);
		float cosradY = cosf(radY);

		//後ろを向いているとき
		if (paterncount == LANDINGPOINT_BACK ||
			paterncount == MOVEDPOINT_A ||
			paterncount == MOVEDPOINT_A_LEFT ||
			paterncount == GOALPOINT_BACK) {
			pos.x = gunworldpos_.m128_f32[0] + sinradX * 3.5f;
			pos.y = gunworldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gunworldpos_.m128_f32[2] + 3.0f;
		}
		//右を向いているとき
		else if (paterncount == MOVEDPOINT_C ||
			paterncount == MOVEDPOINT_C_OBLIQUE) {
			pos.x = gunworldpos_.m128_f32[0] + 2.3f;
			pos.y = gunworldpos_.m128_f32[1] ;
			pos.z = gunworldpos_.m128_f32[2] ;
		}
		//真っすぐ前を向いているとき
		else if (paterncount == LANDINGPOINT_FRONT || 
			paterncount == GOALPOINT) {
			pos.x = gunworldpos_.m128_f32[0] - sinradX * 3.5f;
			pos.y = gunworldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gunworldpos_.m128_f32[2] - 3.0f;
		}
		//左を向いているとき
		else if (paterncount == MOVEDPOINT_B) {
			pos.x = gunworldpos_.m128_f32[0] - 2.3f;
			pos.y = gunworldpos_.m128_f32[1] ;
			pos.z = gunworldpos_.m128_f32[2] + 3.f*sinradX;
		}
		//斜めを向いているとき
		else if (paterncount == MOVEDPOINT_C_FRONT) {
			pos.x = gunworldpos_.m128_f32[0] + 2.3f * sinradX;
			pos.y = gunworldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gunworldpos_.m128_f32[2] + 2.8f * cosradX;
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
		partred_->Add(20, pos, vel, acc, 0.7f, 0.2f, 1.0f);
		partgreen_->Add(20, pos, vel, acc, 0.5f, 0.2f, 1.0f);
		partsmoke_->Add(50, pos, smokevel, acc, 0.5f, 0.0f, 1.0f);
	}
	SoundEffect();

}
//ImgUi描画
void Player::ImGuiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");
	if (ImGui::TreeNode("gunpos")) {
		ImGui::SliderFloat("gunpos.x", &gunpos_.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.y", &gunpos_.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunpos.z", &gunpos_.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("gunWorldpos")) {
		ImGui::SliderFloat("gunWordpos.x", &gunnotparentpos_.m128_f32[0], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.y", &gunnotparentpos_.m128_f32[1], -100.0f, 100.0f);
		ImGui::SliderFloat("gunWordpos.z", &gunnotparentpos_.m128_f32[2], -100.0f, 100.0f);
		ImGui::TreePop();
	}


	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}