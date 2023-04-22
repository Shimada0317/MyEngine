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
void Player::StatusSet(Camera* camera, XMFLOAT3 eyerot)
{
	for (int i = 0; i < MaxRemainingBullet; i++) {
		//落ちるフラグがtrueなら薬莢を下に落とす
		if (dropbulletflag_[i] == true) {
			spritepos_[i].y += addspriteposition;
			time_[i] += 0.5f;
			spritepos_[i].x += Action::GetInstance()->GetRangRand(-10, 10);
			Action::GetInstance()->ThrowUp(Gravity, time_[i], 40, spritepos_[i].y);
			spriterot_[i] += 80;
		}
		//落ちたスプライトが画面外に出たらtime_を0にする
		else if (spritepos_[i].y > WinApp::window_height * 2) {
			time_[i] = {};
		}
	}
	
	//本体のワールド座標取得
	bodymat_ = body_->GetMatrix();
	bodyworldpos_ = { -10.0f,0.0f,-20.0f };
	bodyworldpos_ = XMVector3Transform(bodyworldpos_, bodymat_);
	//レティクルの位置から角度の算出
	changerot_ = eyerot;
	reticlerot_.y = (reticlepos2d_.x - WinApp::window_width / 2) / 10 + changerot_.y;
	//本体のパーツのステータスセット
	body_->SetRotation(bodyrot_);
	body_->SetPosition(bodypos_);
	body_->SetScale(bodyscl_);
	body_->SetParent(camera);
	//銃のワールド座標取得とステータスセット
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
		bulletHUD[i]->SetSize(SpriteSiz);
		bulletHUD[i]->SetPosition(spritepos_[i]);
		bulletHUD[i]->SetRotation(spriterot_[i]);
	}
	//リロードの文字
	reload_->SetSize(reloadspritesize_);
	reload_->SetColor(reloadspritecolor_);
}
//オブジェクトなどの更新処理
void Player::AllUpdate()
{
	body_->Update();
	gun_->Update();
	partred_->Update(ColorRed);
	partgreen_->Update(ColorGreen);
	partsmoke_->Update(ColorSmoke);
}
//更新処理
void Player::Update(Camera* camera, Phase patern, XMFLOAT3 eyerot)
{

	MouseContoroll();

	WaitProcess();

	GunShotProcess(patern);

	UIMotionProcess();

	ReloadProcess();

	velocity_ = XMVector3TransformNormal(velocity_, bodymat_);

	StatusSet(camera, eyerot);

	AllUpdate();
}
//待機状態の処理
void Player::WaitProcess()
{
	//ステータスが待機状態の時
	if (playerstate_ == WAIT) {
		gunrot_.x = (reticlepos2d_.y - WinApp::window_height / 2) / 50;
	}
}
//パーティクル描画
void Player::ParticleDraw(ID3D12GraphicsCommandList* cmdeList)
{
	//マズルフラッシュの描画
	ParticleManager::PreDraw(cmdeList);
	partsmoke_->Draw();
	partred_->Draw();
	partgreen_->Draw();
	ParticleManager::PostDraw();
}
//スプライト描画
void Player::SpriteDraw()
{
	if (!mousestopflag_) {
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
	if (recoilgunflag_) {
		reticlepos2d_.y += 15;
		Mouse::GetInstance()->RecoilMouse(reticlepos2d_);
	}
	else {
		//取得した座標をレティクルにセット
		spritereticle_->SetPosition(reticlepos2d_);
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
	//ステータスがSHOtに切り替わった時
	if (playerstate_ == SHOT) {
		//弾が発射された
		bulletshotflag_ = true;
		playerstate_ = WAIT;
	}
	//それ以外の時
	else {
		bulletshotflag_ = false;
	}
	//残弾が減った時
	if (oldremaining_ < remaining_) {
		dropbulletflag_[oldremaining_] = true;
		oldremaining_ = remaining_;
	}
	//リコイル処理
	RecoilProcess();
}

void Player::UIMotionProcess()
{
	//もし残弾が0になったら
	if (remaining_ > MaxRemainingBullet) {
		//反転フラグがtrueの時
		if (!reversflag_) {
			//Reloadの文字を徐々に大きくする
			Action::GetInstance()->EaseOut(reloadspritesize_.x, 260);
			Action::GetInstance()->EaseOut(reloadspritesize_.y, 190);
			if (reloadspritesize_.x >= 250) {
				reversflag_ = true;
			}
		}
		//反転フラグがfalseのとき
		else {
			//Reloadの文字を徐々に小さくする
			Action::GetInstance()->EaseOut(reloadspritesize_.x, 200);
			Action::GetInstance()->EaseOut(reloadspritesize_.y, 130);
			if (reloadspritesize_.x <= 210) {
				reversflag_ = false;
			}
		}
	}
	//残弾が満タンのの時
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
	const float addrecovery_ = 0.2f;
	const int bouncerotation_ = 25;
	//後ろに後退した値
	const float recession = 3.f;
	//recoverytimeの上限値
	const int timelimit_ = 1;
	//リコイルフラグがtrueの時
	if (recoilgunflag_) {
		recoverytime_ += addrecovery_;
		gunrot_.x = -bouncerotation_;
		gunpos_.m128_f32[2] = -recession;
		if (recoverytime_ >= timelimit_) {
			gunrot_.x = {};
			gunpos_.m128_f32[2] = -recession;
			recoverytime_ = {};
			recoilgunflag_ = false;
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
	if (playerstate_ == WAIT && remaining_ != 0) {
		//右クリックした時
		if (Mouse::GetInstance()->PushClick(1)) {
			//ステータスをRELOADに変更
			playerstate_ = RELOAD;
			reloadse_->LoadFile("Resources/Sound/SE/reload.wav", 0.3f);
			mousestopflag_ = true;
		}
	}
	//ステータスがRELOADの時
	if (playerstate_ == RELOAD) {
		gunrot_.x -= subrotation_;
		remaining_ = emptyremaining_;
		reloadtime_ += addtime_;
		anser_ = reloadtime_ % divtime_;
		//reloaadtime/40の余りが0の時
		if (anser_ == 0) {
			remaining_ = {};
			gunrot_.x = {};
			//残弾が満タンになった時
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
			pos.y = gunworldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gunworldpos_.m128_f32[2] + 2.8f * cosradX;
		}
		//真っすぐ前を向いているとき
		else if (paterncount == LANDINGPOINT_FRONT ||
			paterncount == GOALPOINT) {
			pos.x = gunworldpos_.m128_f32[0] + sinradX * 3.5f;
			pos.y = gunworldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gunworldpos_.m128_f32[2] - 2.0f;
		}
		//左を向いているとき
		else if (paterncount == MOVEDPOINT_B) {
			pos.x = gunworldpos_.m128_f32[0] - 2.3f;
			pos.y = gunworldpos_.m128_f32[1] - sinradY * 1.5f;
			pos.z = gunworldpos_.m128_f32[2] + 2.8f * cosradX;
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
	if (ImGui::TreeNode("change")) {
		ImGui::SliderFloat("changerot.x", &changerot_.x, -100.0f, 100.0f);
		ImGui::SliderFloat("changerot.y", &changerot_.y, -100.0f, 100.0f);
		ImGui::SliderFloat("changerot.z", &changerot_.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}


	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}