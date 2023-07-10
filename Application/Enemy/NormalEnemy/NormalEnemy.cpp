#include "NormalEnemy.h"
#include"Action.h"
#include"Collision.h"
#include"HelperMath.h"
#include"Player.h"
#include"SpriteManager.h"

using namespace DirectX;
const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.1f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
const float attacktime_min_ = 10;
const float attacktime_max_ = 20;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };

const XMFLOAT4 operator+(const DirectX::XMFLOAT4& lhs, const DirectX::XMFLOAT4& rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

//デストラクタ
NormalEnemy::~NormalEnemy()
{
	shadow_.reset();
	center_.reset();
	headpart_.reset();
	bodypart_.reset();
	armspart_.reset();
}

void(NormalEnemy::* NormalEnemy::StateFuncTable[])() {
	&NormalEnemy::Defomation,
		& NormalEnemy::TrackPlayerMode,
		& NormalEnemy::WaitMode,
		& NormalEnemy::AttackMode,
		& NormalEnemy::Death,

};

//初期化処理
void NormalEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint)
{
	state_ = State::kDefomation;
	headpart_rot_ = bodypart_rot_ = armspart_rot_ = allrot;
	all_pos_ = allpos;
	bringupcamera_ = camera;
	purse_positiverot_ += headpart_rot_.y;
	origin_distance_ = distance_;
	originhead_distance_ = head_distance_;

	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	headpart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kHead));
	bodypart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kBody));
	armspart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kArms));
	partgreen_ = ParticleManager::Create(bringupcamera_);
	partred_ = ParticleManager::Create(bringupcamera_);

	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(all_pos_, center_mat_);

	rockon_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_pos_, rockon_color_, anchorpoint_));
	rockonhead_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockonhead_pos_, rockon_color_, anchorpoint_));

	headpart_scl_ = { 0.0f,0.0f,0.0f };
	armspart_scl_ = { 0.0f,0.0f,0.0f };
	track_point_ = oldtrack_point_ = trackpoint;
	hp_ = 160;
	oldhp_ = hp_;
	timer_limit_ = 8;
	center_->SetPosition(center_worldpos_);
}

//ステータスセット
void NormalEnemy::StatusSet()
{

	center_->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(center_mat_);
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(all_pos_, center_mat_);
	center_->SetPosition(center_worldpos_);

	shadow_pos_ = center_worldpos_;
	shadow_pos_.m128_f32[1] = -0.8f;
	shadow_->SetPosition(shadow_pos_);
	shadow_->SetRotation({ 0.0f,0.0f,0.0f });
	shadow_->SetScale({ 1.0f,1.0f,1.0f });

	headpart_pos_ = armspart_pos_ = bodypart_pos_ = center_worldpos_;
	headpart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] + 1.0f;
	armspart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] + 0.2f;

	headpart_->SetPosition(headpart_pos_);
	headpart_->SetRotation(headpart_rot_);
	headpart_->SetScale(headpart_scl_);

	bodypart_->SetPosition(bodypart_pos_);
	bodypart_->SetRotation(bodypart_rot_);
	bodypart_->SetScale(bodypart_scl_);

	armspart_->SetPosition(armspart_pos_);
	armspart_->SetRotation(armspart_rot_);
	armspart_->SetScale(armspart_scl_);

	rockon_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, bodypart_pos_, bringupcamera_);
	rockonhead_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, headpart_pos_, bringupcamera_);
	rockon_->SetPosition(rockon_pos_);
	rockonhead_->SetPosition(rockonhead_pos_);
}

//Obj等の更新処理をまとめる
void NormalEnemy::AllUpdate()
{
	shadow_->Update(shadow_color_);
	center_->Update();

	headpart_->Update(headpart_color_);
	bodypart_->Update(bodypart_color_);
	armspart_->Update(armspart_color_);

	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });

	for (std::unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update();
	}
}
//更新処理
void NormalEnemy::Update(Player* player)
{

	player_ = player;
	player_hp_ = player->GetHp();
	bool playershot = player->GetBulletShot();
	player_pos_ = player->GetRetPosition();
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});
	//当たり判定
	if (playershot == true && hp_ > 0) {
		if (Collision::GetInstance()->CheckHit2D(player_pos_, rockon_pos_, distance_, 1.3f)) {
			hp_ -= BodyDamage;
			playershot = false;
		}
		if (Collision::GetInstance()->CheckHit2D(player_pos_, rockonhead_pos_, head_distance_, 1.3f)) {
			hp_ -= HeadDamage;
			playershot = false;
		}
	}
	//関数ポインタで状態遷移
	(this->*StateFuncTable[state_])();
	Damage();

	StatusSet();
	AllUpdate();
	if (hp_ > 0) { return; }
	state_ = State::kDeath;
}

//描画処理
void NormalEnemy::Draw(DirectXCommon* dxCommon)
{
	ParticleManager::PreDraw(dxCommon->GetCmdList());
	partgreen_->Draw();
	partred_->Draw();
	ParticleManager::PostDraw();

	Object3d::PreDraw(dxCommon->GetCmdList());
	for (std::unique_ptr<ObjParticle>& particle : obj_particle_) {
		particle->Draw();
	}

	headpart_->Draw();
	bodypart_->Draw();
	armspart_->Draw();
	shadow_->Draw();
	Object3d::PostDraw();
}

void NormalEnemy::Defomation()
{
	//変形前なら
	all_pos_.m128_f32[1] -= FallSpeed;
	//地面に着いたとき
	if (all_pos_.m128_f32[1] <= 0) {
		all_pos_.m128_f32[1] = 0;
		defomation_count_ += AddDefomationValue;
		if (headpart_scl_.z <= 0.3f && armspart_scl_.z <= 0.2f) {
			DeploymentScale();
		}
	}

	if (defomation_count_ <= 1) { return; }

	state_ = State::kMove;
}
//プレイヤーへの追尾モードの時
void NormalEnemy::TrackPlayerMode()
{
	//追尾の計算
	XMFLOAT3 Value;
	Value = HelperMath::GetInstance()->TrackCalculation(all_pos_, track_point_);
	//値を2乗
	XMFLOAT3 SquareValue{};
	SquareValue = HelperMath::GetInstance()->SquareToXMFLOAT3(Value, 2);
	//距離の計算
	length_ = HelperMath::GetInstance()->LengthCalculation(SquareValue);
	//追尾速度の計算
	XMVECTOR TrackSpeed{};
	TrackSpeed = HelperMath::GetInstance()->TrackingVelocityCalculation(Value, length_, movespeed_);
	//距離をもとの値に戻す
	distance_ = origin_distance_;
	head_distance_ = originhead_distance_;
	//距離の計算
	distance_ -= length_ * 2.0f;
	head_distance_ -= length_;
	//追尾移動
	HelperMath::GetInstance()->TrackEnemytoPlayer(all_pos_, TrackSpeed);
	if (length_ <= limit_length_ && wait_flag_ == false) {
		bodypart_pos_.m128_f32[2] -= 1.f;
		state_ = State::kWait;
		timer_limit_ = Action::GetInstance()->GetRangRand(attacktime_min_, attacktime_max_);
	}
}
//攻撃モードの時
void NormalEnemy::WaitMode()
{
	atttack_timer_ += 0.1f;
	if (atttack_timer_ >= timer_limit_) {
		Action::GetInstance()->EaseOut(headpart_rot_.y, purse_positiverot_ + 1);
		if (headpart_rot_.y >= purse_positiverot_) {
			headpart_rot_.y = purse_positiverot_;
			state_ = State::kAttack;
		}
	}
}

//攻撃する時
void NormalEnemy::AttackMode()
{
	AttackCharge();
}

void NormalEnemy::Damage()
{
	//ダメージを受けたとき
	if (oldhp_ > hp_) {
		oldhp_ = hp_;
		HitColor();
		for (int i = 0; i < 5; i++) {
			std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
			newparticle->Initialize(1, all_pos_, { 0.3f,0.3f,0.3f }, { bodypart_rot_ });
			obj_particle_.push_back(std::move(newparticle));
		}
	}
}

void NormalEnemy::Death()
{
	
	if (headpart_color_.w >= 0) {
		Transparentize();
		ParticleEfect();
	}
	if (obj_particle_.empty() && shadow_color_.w < 0) {
		dead_flag_ = true;
	}
}

void NormalEnemy::DeploymentScale()
{
	Action::GetInstance()->EaseOut(headpart_scl_.x, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.y, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.z, 1.0f);

	Action::GetInstance()->EaseOut(armspart_scl_.x, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.y, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.z, 0.8f);
}

void NormalEnemy::ParticleEfect()
{
	for (int i = 0; i < 10; i++) {
		XMFLOAT3 pos;
		pos = HelperMath::GetInstance()->ConvertToXMVECTOR(bodypart_pos_);

		const float rnd_vel = 0.04f;
		XMFLOAT3 Vel{};
		XMFLOAT3 MinValue{ -0.09f,-0.01f,-0.03f };
		XMFLOAT3 MaxValue{ 0.09f,0.12f,0.09f };
		Vel = Action::GetInstance()->RandMinAndMax(MinValue, MaxValue);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		partred_->Add(30, pos, Vel, acc, 1.0f, 0.0f, 0.0f);
		partgreen_->Add(30, pos, Vel, acc, 0.7f, 0.0f, 0.0f);
	}
}

void NormalEnemy::WaitTrack(bool otherenemyarive)
{
	if (otherenemyarive == true) {
		limit_length_ = 2.5f;
		oldtrack_point_.m128_f32[2] = oldtrack_point_.m128_f32[2] - 2;
		wait_flag_ = true;
	}
	else {
		limit_length_ = 1.5f;
		oldtrack_point_ = track_point_;
		wait_flag_ = false;
		atttack_timer_ = 0;
	}
}
//巨大化
void NormalEnemy::Enlargement()
{
	//巨大化していく値
	XMFLOAT3 gigantic = { 0.0002f ,0.0002f ,0.0002f };
	armspart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(armspart_scl_, gigantic);
	bodypart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(bodypart_scl_, gigantic);
	headpart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(headpart_scl_, gigantic);
}

void NormalEnemy::ShakeBody()
{
	//震える値の反転
	if (vibrationchange_flag_ == true) {
		vibration_ -= 4.2f;
		if (vibration_ <= -4.2f) {
			vibrationchange_flag_ = false;
		}
	}
	else {
		vibration_ += 4.2f;
		if (vibration_ >= 4.2f) {
			vibrationchange_flag_ = true;
		}
	}
	//体の震え
	headpart_rot_.y += vibration_;
	bodypart_rot_.y += vibration_;
	armspart_rot_.y += vibration_;
}

void NormalEnemy::AttackCharge()
{
	AttackBefore();
	limit_length_ = 0.1f;
	//振動させる
	ShakeBody();
	attack_charge_ += 0.1f;
	if (attack_charge_ > 9) {
		Attack();
	}
}

void NormalEnemy::AttackBefore()
{
	const float discoloration = 0.05f;
	const float MaxPoint = 40.f;
	if (armspart_rot_.x > MaxPoint) { return; }
	armspart_rot_.x += 1.5f;
	Enlargement();
	armspart_color_.y -= discoloration;
	armspart_color_.z -= discoloration;
}

void NormalEnemy::Attack()
{
	armspart_rot_.x -= 10.0f;
	if (armspart_rot_.x <= 0.0f) {
		player_hp_ -= 1;
		player_->SetHp(player_hp_);
		hp_ = 0;
		state_ = State::kDeath;
	}
}

void NormalEnemy::Transparentize()
{
	shadow_color_.w -= Subtraction;
	armspart_color_.w -= Subtraction;
	bodypart_color_.w -= Subtraction;
	headpart_color_.w -= Subtraction;
}

void NormalEnemy::HitColor()
{
	float subvalue = 0.2f;
	headpart_color_.y -= subvalue;
	headpart_color_.z -= subvalue;
	bodypart_color_.y -= subvalue;
	bodypart_color_.z -= subvalue;
}
