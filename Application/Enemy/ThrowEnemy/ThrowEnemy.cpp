#include "ThrowEnemy.h"
#include"Action.h"
#include"Collision.h"
#include"HelperMath.h"
#include"ModelManager.h"
#include"SpriteManager.h"
#include"Player.h"

using namespace DirectX;

const int BodyDamage = 25;
const float FallSpeed = 0.15f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


ThrowEnemy::~ThrowEnemy()
{
	enemy_.reset();
	propeller_.reset();
}
void (ThrowEnemy::* ThrowEnemy::StateFuncTable[])() {
	&ThrowEnemy::AppearanceProcess,
		&ThrowEnemy::WaitProcess,
		&ThrowEnemy::AttackProcess,
		&ThrowEnemy::DeathProcess,
};


//初期化処理
void ThrowEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos)
{
	bringupcamera_ = camera;
	//オブジェクトの生成
	enemy_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kThrowEnemy));
	propeller_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kEnemyPropeller));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	bullet_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSphere));
	//スプライトの生成
	rockon_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_pos_, rockon_color_, anchorpoint_));
	rockon_bullet_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_bulletpos_, rockon_color_, anchorpoint_));

	partgreen_ = ParticleManager::Create(bringupcamera_);
	partred_ = ParticleManager::Create(bringupcamera_);
	floating_pos_ = Action::GetInstance()->GetRangRand(6.f, 7.f);
	body_rot_ = allrot;
	body_rot_.x -= 10;
	center_pos_ = allpos;
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(center_pos_, center_mat_);
	landing_point_ = trackpos;
	oldhp_ = hp_;
	center_->SetPosition(center_worldpos_);
}
//ステータスのセット
void ThrowEnemy::StatusSet()
{
	center_->SetScale({ 1.f,1.f,1.f });
	XMMatrixIsIdentity(center_mat_);
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(center_pos_, center_mat_);
	center_->SetPosition(center_worldpos_);

	body_pos_ = propeller_pos_ = center_worldpos_;

	enemy_->SetPosition(body_pos_);
	enemy_->SetRotation(body_rot_);
	enemy_->SetScale(body_scl_);

	bullet_->SetPosition(bullet_pos_);
	bullet_->SetRotation(bullet_rot_);
	bullet_->SetScale(bullet_scl_);

	propeller_rot_.y += 15;
	propeller_->SetPosition(propeller_pos_);
	propeller_->SetRotation(propeller_rot_);
	propeller_->SetScale(propeller_scl_);

	rockon_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, body_pos_, bringupcamera_);
	rockon_->SetPosition(rockon_pos_);
	rockon_bulletpos_ = Action::GetInstance()->WorldToScreen(center_mat_, bullet_pos_, bringupcamera_);
	rockon_bullet_->SetPosition(rockon_bulletpos_);
}
//全ての更新処理をまとめる
void ThrowEnemy::AllUpdate()
{
	enemy_->Update(color_);
	propeller_->Update(color_);
	center_->Update();
	bullet_->Update(bullet_color_);

	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });
	for (unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update();
	}
}
//更新処理
void ThrowEnemy::Update(Player* player)
{
	player_ = player;
	player_pos_ = player->GetRetPosition();
	player_shot_ = player->GetBulletShot();
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});
	//関数ポインタで状態遷移
	(this->*StateFuncTable[state_])();
	//ダメージを食らったときの処理
	DamageProcess();
	if (hp_ <= 0) {
		state_ = State::DEATH;
	}
	//ステータスのセット
	StatusSet();
	//全ての更新処理をまとめる
	AllUpdate();
}
//登場処理
void ThrowEnemy::AppearanceProcess()
{
	//落下してくる
	center_pos_.m128_f32[1] -= FallSpeed;
	if (center_pos_.m128_f32[1] <= floating_pos_) {
		bullet_pos_ = center_worldpos_;
		bullet_pos_.m128_f32[1] = bullet_pos_.m128_f32[1] - 1.f;
		old_pos_ = bullet_pos_;
		state_ = State::WAIT;
	}
}
//待機処理
void ThrowEnemy::WaitProcess()
{
	bullet_active_ = true;
	bullet_scl_ = HelperMath::GetInstance()->XMFLOAT3AddFloat(bullet_scl_, 0.005f);
	if (bullet_scl_.z <= 0.3f) { return; }
	state_ = State::ATTACK;
}
//攻撃処理
void ThrowEnemy::AttackProcess()
{
	//弾発射
	ThrowAttack();
	//弾の当たり判定
	BulletCollision();
}
//ダメージを食らったときの処理
void ThrowEnemy::DamageProcess()
{
	float Vx = 0;
	float Vy = 0;
	float Vz = 0;

	Vx = (bullet_pos_.m128_f32[0] - landing_point_.m128_f32[0]);
	Vy = (bullet_pos_.m128_f32[1] - landing_point_.m128_f32[1] - 2);
	Vz = (bullet_pos_.m128_f32[2] - landing_point_.m128_f32[2]);

	float V2x = powf(Vx, 2.f);
	float V2y = powf(Vy, 2.f);
	float V2z = powf(Vz, 2.f);

	length_ = sqrtf(V2x + V2y + V2z);
	distance_ = length_;

	//当たり判定
	if (player_shot_== true && hp_ > 0) {
		if (!Collision::GetInstance()->CheckHit2D(player_pos_, rockon_pos_, distance_, 2.f)) { return; }
		hp_ -= BodyDamage;
		player_shot_ = false;
	}
	if (hp_ >= oldhp_) { return; }
	for (int i = 0; i < 5; i++) {
		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
		newparticle->Initialize(1, body_pos_, { 0.3f,0.3f,0.3f }, { body_rot_ });
		obj_particle_.push_back(std::move(newparticle));
	}
	oldhp_ = hp_;
}
//死亡処理
void ThrowEnemy::DeathProcess()
{
	const float kGravity = 1.8f;
	fall_time_ += 0.001f;
	float fallspeed = kGravity * fall_time_;
	color_.w -= fall_time_;
	bullet_color_.w -= fall_time_;

	if (center_pos_.m128_f32[1] >= 0) {
		center_pos_.m128_f32[1] -= fallspeed;
		if (center_pos_.m128_f32[1] <= 0) {
			dead_flag_ = true;
		}
	}
	if (color_.w <= 0) { return; }
	ParticleEfect();
}

//描画処理
void ThrowEnemy::Draw(DirectXCommon* dxCommon)
{
	//オブジェクト
	Object3d::PreDraw(dxCommon->GetCmdList());
	for (std::unique_ptr<ObjParticle>& particle : obj_particle_) {
		particle->Draw();
	}
	enemy_->Draw();
	propeller_->Draw();
	if (bullet_active_ == true) {
		bullet_->Draw();
	}
	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCmdList());
	partgreen_->Draw();
	partred_->Draw();
	ParticleManager::PostDraw();
	//スプライト
	Sprite::PreDraw(dxCommon->GetCmdList());
	//rockon_->Draw();
	//rockon_bullet_->Draw();
	Sprite::PostDraw();
}
//弾の当たり判定
void ThrowEnemy::BulletCollision()
{
	
	//弾が発射されている
	if (player_shot_ == false) { return; }
	//当たり判定
	if (!Collision::GetInstance()->CheckHit2D(player_pos_, rockon_bulletpos_, bullet_distance_, bullet_magnification_)) { return; }
	bullet_active_ = false;
	player_shot_ = false;
}
//弾発射
void ThrowEnemy::ThrowAttack()
{
	//追尾の計算
	XMFLOAT3 Value;
	Value.x= bullet_pos_.m128_f32[0] - landing_point_.m128_f32[0];
	Value.y = bullet_pos_.m128_f32[1] - landing_point_.m128_f32[1] - 2;
	Value.z = bullet_pos_.m128_f32[2] - landing_point_.m128_f32[2];
	//値を2乗
	XMFLOAT3 SquareValue{};
	SquareValue = HelperMath::GetInstance()->SquareToXMFLOAT3(Value, 2);
	//距離の計算
	float BulletLength = 0;
	BulletLength = HelperMath::GetInstance()->LengthCalculation(SquareValue);
	//追尾速度の計算
	XMVECTOR TrackSpeed{};
	TrackSpeed = HelperMath::GetInstance()->TrackingVelocityCalculation(Value, BulletLength, bullet_speed_);

	bullet_distance_ = BulletLength;
	bullet_pos_ -= TrackSpeed;
	bullet_magnification_ += 0.015f;
	if (BulletLength <= 0.1f) {
		player_hp_ = player_->GetHp();
		player_hp_ -= 1;
		player_->SetHp(player_hp_);
		bullet_pos_ = old_pos_;
		bullet_scl_ = {};
		state_ = State::WAIT;
		bullet_magnification_ = 0.f;
	}

	if (bullet_active_ == false) {
		bullet_pos_ = old_pos_;
		bullet_scl_ = {};
		state_ = State::WAIT;
	}
}

void ThrowEnemy::ParticleEfect()
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 Pos;
		Pos.x = center_worldpos_.m128_f32[0];
		Pos.y = center_worldpos_.m128_f32[1] - 1.f;
		Pos.z = center_worldpos_.m128_f32[2];

		XMFLOAT3 Vel{};
		XMFLOAT3 MinValue{ -0.09f,-0.11f,-0.09f };
		XMFLOAT3 MaxValue{ 0.09f,0.12f,0.09f };

		Vel = Action::GetInstance()->RandMinAndMax(MinValue, MaxValue);

		XMFLOAT3 Acc{};

		partred_->Add(30, Pos, Vel, Acc, 1.0f, 0.0f, 0.0f);
		partgreen_->Add(30, Pos, Vel, Acc, 0.7f, 0.0f, 0.0f);
	}
}

