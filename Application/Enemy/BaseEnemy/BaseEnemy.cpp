#include "BaseEnemy.h"

#include"Collision.h"
#include"HelperMath.h"
#include"Player.h"
#include"SpriteManager.h"

using namespace DirectX;
const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.1f;
const float AttacktimeMin = 5;
const float AttacktimeMax = 10;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


void BaseEnemy::Initialize(Camera* camera)
{
	bringupcamera_ = camera;
	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	headpart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kHead));
	bodypart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kBody));
	armspart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kArms));
	partgreen_ = ParticleManager::Create(bringupcamera_);
	partred_ = ParticleManager::Create(bringupcamera_);
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(base_pos_, center_mat_);
	center_->SetPosition(center_worldpos_);
	rockon_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_pos_, rockon_color_, anchorpoint_));
	rockonhead_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockonhead_pos_, rockon_color_, anchorpoint_));
}

void BaseEnemy::StatusSet()
{
	center_->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(center_mat_);
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(base_pos_, center_mat_);
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

void BaseEnemy::AllUpdate()
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

void BaseEnemy::Update(Player* player)
{
	player_ = player;
	player_hp_ = player->GetHp();
	bool playershot = player->GetBulletShot();
	player_pos_ = player->GetRetPosition();
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});
	//ìñÇΩÇËîªíË
	if (playershot == true && base_hp_ > 0) {
		if (Collision::GetInstance()->CheckHit2D(player_pos_, rockon_pos_, distance_, 1.3f)) {
			base_hp_ -= BodyDamage;
			playershot = false;
		}
		if (Collision::GetInstance()->CheckHit2D(player_pos_, rockonhead_pos_, head_distance_, 1.3f)) {
			base_hp_ -= HeadDamage;
			playershot = false;
		}
	}
	//äeìGéñÇÃçsìÆëJà⁄
	Activity();
	
	Damage();

	StatusSet();
	
	AllUpdate();
	if (base_hp_ > 0) { return; }
	state_ = State::kDeath;
}

void BaseEnemy::Draw(DirectXCommon* dxCommon)
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

void BaseEnemy::TrackPlayerMode()
{
	//í«îˆÇÃåvéZ
	XMFLOAT3 Value;
	Value = HelperMath::GetInstance()->TrackCalculation(base_pos_, track_point_);
	//ílÇ2èÊ
	XMFLOAT3 SquareValue{};
	SquareValue = HelperMath::GetInstance()->SquareToXMFLOAT3(Value, 2);
	//ãóó£ÇÃåvéZ
	length_ = HelperMath::GetInstance()->LengthCalculation(SquareValue);
	//í«îˆë¨ìxÇÃåvéZ
	XMVECTOR TrackSpeed{};
	TrackSpeed = HelperMath::GetInstance()->TrackingVelocityCalculation(Value, length_, movespeed_);
	//ãóó£ÇÇ‡Ç∆ÇÃílÇ…ñﬂÇ∑
	distance_ = origin_distance_;
	head_distance_ = originhead_distance_;
	//ãóó£ÇÃåvéZ
	distance_ -= length_ * 2.0f;
	head_distance_ -= length_;
	//í«îˆà⁄ìÆ
	HelperMath::GetInstance()->TrackEnemytoPlayer(base_pos_, TrackSpeed);
	if (length_ <= limit_length_ && wait_flag_ == false) {
		bodypart_pos_.m128_f32[2] -= 1.f;
		state_ = State::kWait;
		timer_limit_ = Action::GetInstance()->GetRangRand(AttacktimeMin, AttacktimeMax);
	}
}

void BaseEnemy::Damage()
{
	if (old_hp_ <= base_hp_) { return; }
	old_hp_ = base_hp_;
	HitColor();
	for (int i = 0; i < 5; i++) {
		std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
		newparticle->Initialize(1, base_pos_, { 0.3f,0.3f,0.3f }, { bodypart_rot_ });
		obj_particle_.push_back(std::move(newparticle));
	}
}


void BaseEnemy::Death()
{
	if (headpart_color_.w >= 0) {
		Tracsparentsize();
		ParticleEfect();
	}
	if (obj_particle_.empty() && shadow_color_.w < 0) {
		dead_flag_ = true;
	}
}

bool BaseEnemy::CheckHit(bool& playershot, XMFLOAT2 playerpos, XMFLOAT2 rockonpos, float distance, float mulvalue)
{
	//î≠ñCÇµÇƒÇ¢Ç»Ç¢
	if (!CheckShot(playershot)) { return false; }
	//ç¿ïWÇ™èdÇ»Ç¡ÇƒÇ¢Ç»Ç¢
	if (!Collision::GetInstance()->CheckHit2D(playerpos, rockonpos, distance, mulvalue)) { return false; }
	return true;
}

void BaseEnemy::ParticleEfect()
{
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;
		pos = HelperMath::GetInstance()->ConvertToXMVECTOR(base_pos_);

		const float rnd_vel = 0.04f;
		XMFLOAT3 Vel{};
		XMFLOAT3 MinValue{ -0.09f,-0.01f,-0.03f };
		XMFLOAT3 MaxValue{ 0.09f,0.12f,0.09f };
		Vel = Action::GetInstance()->RandMinAndMax(MinValue, MaxValue);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		partred_->Add(200, pos, Vel, acc, 4.0f, 0.0f, 150.0f);
		partgreen_->Add(200, pos, Vel, acc, 3.7f, 0.0f, 150.0f);
	}
}

void BaseEnemy::Tracsparentsize()
{
	shadow_color_.w -= Subtraction;
	armspart_color_.w -= Subtraction;
	bodypart_color_.w -= Subtraction;
	headpart_color_.w -= Subtraction;
}

void BaseEnemy::HitColor()
{
	float subvalue = 0.2f;
	headpart_color_.y -= subvalue;
	headpart_color_.z -= subvalue;
	bodypart_color_.y -= subvalue;
	bodypart_color_.z -= subvalue;
}

void BaseEnemy::AttackCharge()
{
	AttackBefore();
	limit_length_ = 0.1f;
	//êUìÆÇ≥ÇπÇÈ
	ShakeBody();
	attack_charge_ += 0.1f;
	if (attack_charge_ > 9) {
		Attack();
	}
}

void BaseEnemy::ShakeBody()
{
	//êkÇ¶ÇÈílÇÃîΩì]
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
	//ëÃÇÃêkÇ¶
	headpart_rot_.y += vibration_;
	bodypart_rot_.y += vibration_;
	armspart_rot_.y += vibration_;
}

void BaseEnemy::Attack()
{
	armspart_rot_.x -= 10.0f;
	if (armspart_rot_.x <= 0.0f) {
		player_hp_ -= 1;
		player_->SetHp(player_hp_);
		base_hp_ = 0;
		state_ = State::kDeath;
	}
}

void BaseEnemy::Transparentize()
{
	shadow_color_.w -= Subtraction;
	armspart_color_.w -= Subtraction;
	bodypart_color_.w -= Subtraction;
	headpart_color_.w -= Subtraction;
}

void BaseEnemy::Enlargement()
{
	//ãêëÂâªÇµÇƒÇ¢Ç≠íl
	XMFLOAT3 gigantic = { 0.0002f ,0.0002f ,0.0002f };
	armspart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(armspart_scl_, gigantic);
	bodypart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(bodypart_scl_, gigantic);
	headpart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(headpart_scl_, gigantic);
}

void BaseEnemy::AttackBefore()
{
	const float discoloration = 0.05f;
	const float MaxPoint = 40.f;
	if (armspart_rot_.x > MaxPoint) { return; }
	armspart_rot_.x += 1.5f;
	Enlargement();
	armspart_color_.y -= discoloration;
	armspart_color_.z -= discoloration;
}

void BaseEnemy::DeploymentScale()
{
	Action::GetInstance()->EaseOut(headpart_scl_.x, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.y, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.z, 1.0f);

	Action::GetInstance()->EaseOut(armspart_scl_.x, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.y, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.z, 0.8f);
}

bool BaseEnemy::CheckShot(bool playershot)
{
	if (!playershot) { return false; }
	return true;
}
