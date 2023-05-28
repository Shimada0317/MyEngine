#include "ThrowEnemy.h"
#include"Action.h"
#include"HelperMath.h"
#include"ModelManager.h"
#include"SpriteManager.h"

using namespace DirectX;

const int BodyDamage = 25;
const float FallSpeed = 0.15f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


ThrowEnemy::~ThrowEnemy()
{
	enemy_.reset();
	propeller_.reset();
}
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

	rockon_pos_ = WorldtoScreen(body_pos_);
	rockon_->SetPosition(rockon_pos_);
	rockon_bulletpos_ = WorldtoScreen(bullet_pos_);
	rockon_bullet_->SetPosition(rockon_bulletpos_);
}
//全ての更新処理をまとめる
void ThrowEnemy::AllUpdate()
{
	enemy_->Update();
	propeller_->Update();
	center_->Update();
	bullet_->Update({1.f,0.f,0.f,1.f});

	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });
	for (unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update();
	}
}
//更新処理
void ThrowEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});
	//登場処理
	AppearanceProcess();
	//待機処理
	WaitProcess();
	//攻撃処理
	AttackProcess(player2Dpos,playerhp,playerbulletshot);
	//ダメージを食らったときの処理
	DamageProcess(player2Dpos,playerbulletshot);
	//死亡処理
	DeathProcess();
	if (Input::GetInstance()->PushKey(DIK_O)) {
		hp_ = 0;
	}
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
	if (state_ != State::APPEARANCE) { return; }
	//落下してくる
	center_pos_.m128_f32[1] -= FallSpeed;
	if (center_pos_.m128_f32[1] <= floating_pos_) {
		bullet_pos_ = center_worldpos_;
		bullet_pos_.m128_f32[1] =bullet_pos_.m128_f32[1] - 1.f;
		old_pos_ = bullet_pos_;
		state_ = State::WAIT;
	}

}
//待機処理
void ThrowEnemy::WaitProcess()
{
	if (state_ != State::WAIT) { return; }
	bullet_active_ = true;
	bullet_scl_=HelperMath::GetInstance()->XMFLOAT3AddFloat(bullet_scl_, 0.001f);
	if (bullet_scl_.z <= 0.3f) { return; }
	state_ = State::ATTACK;
}
//攻撃処理
void ThrowEnemy::AttackProcess(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	if (state_ != State::ATTACK) { return; }
	//弾発射
	ThrowAttack(playerhp);
	//弾の当たり判定
	BulletCollision(player2Dpos, playerbulletshot);
}
//ダメージを食らったときの処理
void ThrowEnemy::DamageProcess(const XMFLOAT2& player2Dpos, bool& playerbulletshot)
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
	if (playerbulletshot == true && hp_ > 0) {
		if (player2Dpos.x - distance_ * 2.f < rockon_pos_.x && player2Dpos.x + distance_ * 2.f > rockon_pos_.x &&
			player2Dpos.y - distance_ * 2.f < rockon_pos_.y && player2Dpos.y + distance_ * 2.f > rockon_pos_.y) {
			hp_ -= BodyDamage;
			playerbulletshot = false;
		}
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
	if (state_ != State::DEATH) { return; }
	const float kGravity = 1.8f;
	fall_time_ += 0.001f;
	float fallspeed = kGravity * fall_time_;
	if (center_pos_.m128_f32[1] >= 0) {
		center_pos_.m128_f32[1] -= fallspeed;
		if (center_pos_.m128_f32[1] <= 0) {
			dead_flag_ = true;
		}
	}
	ParticleEfect();
}
//3Dから2Dに変換
XMFLOAT2 ThrowEnemy::WorldtoScreen(const XMVECTOR& set3Dposition)
{
	center_->SetRotation(center_rot_);
	center_mat_ = center_->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset_ = { 0.0,0.0,1.0f };
	offset_ = XMVector3TransformNormal(offset_, center_mat_);
	offset_ = XMVector3Normalize(offset_) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = set3Dposition;

	HelperMath::GetInstance()->ChangeViewPort(matviewport_, offset_);

	XMMATRIX MatVP = matviewport_;

	XMMATRIX View = bringupcamera_->GetViewMatrix();
	XMMATRIX Pro = bringupcamera_->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 Get2dPosition;
	Get2dPosition.x = PositionRet.m128_f32[0];
	Get2dPosition.y = PositionRet.m128_f32[1];

	return Get2dPosition;
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
	if (bullet_active_==true) {
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
void ThrowEnemy::BulletCollision(const XMFLOAT2& player2Dpos, bool& playerbulletshot)
{
	//当たり判定
	if (playerbulletshot == false) { return; }
	if (player2Dpos.x - bullet_distance_ * 4.f < rockon_bulletpos_.x && player2Dpos.x + bullet_distance_ * 4.f > rockon_bulletpos_.x &&
		player2Dpos.y - bullet_distance_ * 4.f < rockon_bulletpos_.y && player2Dpos.y + bullet_distance_ * 4.f > rockon_bulletpos_.y) {
		bullet_active_ = false;
		playerbulletshot = false;
	}
}
//弾発射
void ThrowEnemy::ThrowAttack(int& playerhp)
{
	float vx = 0;
	float vy = 0;
	float vz = 0;
	float BulletLength = 0;

	vx = (bullet_pos_.m128_f32[0] - landing_point_.m128_f32[0]);
	vy = (bullet_pos_.m128_f32[1] - landing_point_.m128_f32[1]-2);
	vz = (bullet_pos_.m128_f32[2] - landing_point_.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);

	BulletLength = sqrtf(v2x + v2y + v2z);

	XMVECTOR v3;
	v3.m128_f32[0] = (vx / BulletLength) * bullet_speed_;
	v3.m128_f32[1] = (vy / BulletLength) * bullet_speed_;
	v3.m128_f32[2] = (vz / BulletLength) * bullet_speed_;

	
	bullet_distance_ = BulletLength;

	bullet_pos_ -= v3;
	if (BulletLength <=0.1f) {
		playerhp -= 1;
		bullet_pos_ = old_pos_;
		bullet_scl_={};
		state_ = State::WAIT;
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
		Pos.y = center_worldpos_.m128_f32[1]-1.f;
		Pos.z = center_worldpos_.m128_f32[2];

		XMFLOAT3 Vel{};
		Vel.x = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);
		Vel.y = Action::GetInstance()->GetRangRand(-0.11f, 0.12f);
		Vel.z = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);

		XMFLOAT3 Acc{};
		Acc.y = 0.0;

		partred_->Add(30, Pos, Vel, Acc, 1.0f, 0.0f, 0.0f);
		partgreen_->Add(30, Pos, Vel, Acc, 0.7f, 0.0f, 0.0f);
	}
}
