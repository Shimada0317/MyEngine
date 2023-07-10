#include "Normal.h"
#include"Player.h"


void (Normal::* Normal::StateFuncTable[])() {
	&Normal::Defomation,
		&Normal::TrackPlayerMode,
		&Normal::WaitMode,
		&Normal::AttackMode,
		&Normal::Death,
};

Normal::Normal(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint)
{
	headpart_rot_ = bodypart_rot_ = armspart_rot_ = allrot;
	base_pos_ = allpos;
	purse_positiverot_ += headpart_rot_.y;
	origin_distance_ = distance_;
	originhead_distance_ = head_distance_;
	headpart_scl_ = { 0.0f,0.0f,0.0f };
	armspart_scl_ = { 0.0f,0.0f,0.0f };
	damage_value_ = 50;

	track_point_ = oldtrack_point_ = trackpoint;
	hp_ = 160;
	old_hp_ = hp_;
	timer_limit_ = 8;
	bodypart_color_ = { 1.f,0.f,0.f,1.f };
	armspart_color_ = { 1.f,0.f,0.f,1.f };
	speed_ = 0.09f;
}

void Normal::CreateRobot(Camera* camera)
{
	bringupcamera_ = camera;
	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	head_part_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kHead));
	body_part_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kBody));
	arms_part_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kArms));
	CommonLoad();

	StatusSet();
}

void Normal::StatusSet()
{
	CommonStatusSet();

	headpart_pos_ = armspart_pos_ = bodypart_pos_ = center_worldpos_;
	headpart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] + 1.0f;
	armspart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] + 0.2f;

	head_part_->SetPosition(headpart_pos_);
	head_part_->SetRotation(headpart_rot_);
	head_part_->SetScale(headpart_scl_);

	body_part_->SetPosition(bodypart_pos_);
	body_part_->SetRotation(bodypart_rot_);
	body_part_->SetScale(bodypart_scl_);

	arms_part_->SetPosition(armspart_pos_);
	arms_part_->SetRotation(armspart_rot_);
	arms_part_->SetScale(armspart_scl_);

	rockon_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, bodypart_pos_, bringupcamera_);
	rockonother_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, headpart_pos_, bringupcamera_);
	rockon_body_->SetPosition(rockon_pos_);
	rockon_other_->SetPosition(rockonother_pos_);
}

void Normal::AllUpdate()
{
	head_part_->Update(headpart_color_);
	body_part_->Update(bodypart_color_);
	arms_part_->Update(armspart_color_);

	CommonUpdate();
}

void Normal::Activity(Player* player)
{

	GetPlayerState(player);

	(this->*StateFuncTable[state_])();

	Damage();

	EnemyCollision();

	ObjParticleDelete();

	AllUpdate();

	StatusSet();

	if (hp_ > 0) { return; }
	state_ = State::kDeath;
}

void Normal::Draw(DirectXCommon* dxCommon)
{
	CommonParticleDraw(dxCommon);

	Object3d::PreDraw(dxCommon->GetCmdList());

	CommonObjDraw();

	head_part_->Draw();
	body_part_->Draw();
	arms_part_->Draw();
	shadow_->Draw();
	Object3d::PostDraw();

	//スプライト
	Sprite::PreDraw(dxCommon->GetCmdList());
	rockon_body_->Draw();
	rockon_other_->Draw();
	Sprite::PostDraw();

}

void Normal::Defomation()
{
	const float FallSpeed = 0.25f;
	const float AddDefomationValue = 0.04f;
	//変形前なら
	base_pos_.m128_f32[1] -= FallSpeed;
	//地面に着いたとき
	if (base_pos_.m128_f32[1] <= 0) {
		base_pos_.m128_f32[1] = 0;
		defomation_count_ += AddDefomationValue;
		if (headpart_scl_.z <= 0.3f && armspart_scl_.z <= 0.2f) {
			DeploymentScale();
		}
	}
	if (defomation_count_ <= 1) { return; }
	state_ = State::kMove;
}

void Normal::TrackPlayerMode()
{

	const float AttackTimeMin = 10;
	const float AttackTimeMax = 20;
	TrackCalculation();

	//距離をもとの値に戻す
	distance_ = origin_distance_;
	head_distance_ = originhead_distance_;
	//距離の計算
	distance_ -= length_ * 2.0f;
	head_distance_ -= length_;
	//追尾移動
	HelperMath::GetInstance()->TrackEnemytoPlayer(base_pos_, TrackSpeed);
	if (length_ <= limit_length_ && wait_flag_ == false) {
		bodypart_pos_.m128_f32[2] -= 1.f;
		state_ = State::kWait;
		timer_limit_ = Action::GetInstance()->GetRangRand(AttackTimeMin, AttackTimeMax);
	}
}

void Normal::WaitMode()
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

void Normal::AttackMode()
{
	AttackCharge();
}

void Normal::Death()
{
	if (headpart_color_.w >= 0) {
		Transparentize();
		ParticleEffect();
	}
	if (obj_particle_.empty() && shadow_color_.w < 0) {
		dead_flag_ = true;
	}
}

void Normal::DeploymentScale()
{
	Action::GetInstance()->EaseOut(headpart_scl_.x, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.y, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.z, 1.0f);

	Action::GetInstance()->EaseOut(armspart_scl_.x, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.y, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.z, 0.8f);
}

void Normal::WaitTrack(bool otherenemyarive)
{
}

void Normal::Enlargement()
{
	//巨大化していく値
	XMFLOAT3 gigantic = { 0.0002f ,0.0002f ,0.0002f };
	armspart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(armspart_scl_, gigantic);
	bodypart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(bodypart_scl_, gigantic);
	headpart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(headpart_scl_, gigantic);
}

void Normal::ShakeBody()
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

void Normal::AttackCharge()
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

void Normal::AttackBefore()
{
	const float discoloration = 0.05f;
	const float MaxPoint = 40.f;
	if (armspart_rot_.x > MaxPoint) { return; }
	armspart_rot_.x += 1.5f;
	Enlargement();
	armspart_color_.y -= discoloration;
	armspart_color_.z -= discoloration;
}

void Normal::Attack()
{
	armspart_rot_.x -= 10.0f;
	if (armspart_rot_.x <= 0.0f) {
		player_hp_ -= 1;
		player_->SetHp(player_hp_);
		hp_ = 0;
		state_ = State::kDeath;
	}
}

void Normal::HitColor()
{
	float SubValue = 0.2f;
	headpart_color_.y -= SubValue;
	headpart_color_.z -= SubValue;
	bodypart_color_.y -= SubValue;
	bodypart_color_.z -= SubValue;
}

void Normal::Transparentize()
{
	const float Subtraction = 0.1f;
	shadow_color_.w -= Subtraction;
	armspart_color_.w -= Subtraction;
	bodypart_color_.w -= Subtraction;
	headpart_color_.w -= Subtraction;
}

void Normal::EnemyCollision()
{
	const int HeadDamage = 80;

	if (player_shot_ == true && hp_ > 0) {
		CommonCollision();
		if (Collision::GetInstance()->CheckHit2D(player_pos_, rockonother_pos_, head_distance_, 1.3f)) {
			hp_ -= HeadDamage;
			player_shot_ = false;
		}
	}
}



