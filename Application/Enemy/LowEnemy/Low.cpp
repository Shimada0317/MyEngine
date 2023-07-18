#include "Low.h"
#include"Player.h"

void (Low::* Low::StateFuncTable[])() {
	&Low::Defomation,
		& Low::TrackPlayerMode,
		& Low::Death,
};

Low::Low(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint,int type)
{
	damage_value_ = 80;
	mul_value_ = 1.f;
	speed_ = 0.1f;
	headpart_rot_ = armspart_rot_ = allrot;
	base_pos_ = allpos;
	origin_distance_ = distance_;
	headpart_scl_ = { 0.0f,0.0f,0.0f };
	armspart_scl_ = { 0.0f,0.0f,0.0f };
	track_point_ = oldtrack_point_ = trackpoint;
	hp_ = 160;
	old_hp_ = hp_;
	if (type == EnemyType::kHighJumpLow) {
		speed_ = 0.05f;
		gravity_ = 0.7f;
	}
}

void Low::CreateRobot(Camera* camera)
{
	bringupcamera_ = camera;
	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	headpart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kHead));
	armspart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kArms));
	legspart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kArms));
	CommonLoad();

	StatusSet();

}

void Low::StatusSet()
{
	CommonStatusSet();

	headpart_pos_ = center_worldpos_;
	armspart_pos_ = center_worldpos_;
	legspart_pos_ = center_worldpos_;
	armspart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] - 0.3f;
	legspart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] - 0.3f;
	legspart_pos_.m128_f32[2] = center_worldpos_.m128_f32[2] - 1.2f;

	headpart_->SetPosition(headpart_pos_);
	headpart_->SetRotation(headpart_rot_);
	headpart_->SetScale(headpart_scl_);

	armspart_->SetPosition(armspart_pos_);
	armspart_->SetRotation(armspart_rot_);
	armspart_->SetScale(armspart_scl_);

	legspart_->SetPosition(legspart_pos_);
	legspart_->SetRotation(armspart_rot_);
	legspart_->SetScale(armspart_scl_);

	rockon_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, base_pos_, bringupcamera_);
	rockon_body_->SetPosition(rockon_pos_);
}

void Low::AllUpdate()
{
	headpart_->Update(headpart_color_);
	armspart_->Update(armspart_color_);
	legspart_->Update(armspart_color_);

	CommonUpdate();
}

void Low::Activity(Player* player)
{
	GetPlayerState(player);

	(this->*StateFuncTable[state_])();

	Damage();

	EnemyCollsion();

	ObjParticleDelete();

	AllUpdate();

	StatusSet();

	if (hp_ > 0) { return; }
	state_ = State::kDeath;
}

void Low::Draw(DirectXCommon* dxCommon)
{
	CommonParticleDraw(dxCommon);

	Object3d::PreDraw(dxCommon->GetCmdList());
	
	CommonObjDraw();

	headpart_->Draw();
	armspart_->Draw();
	legspart_->Draw();
	shadow_->Draw();
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	rockon_body_->Draw();
	Sprite::PostDraw();
}

void Low::Defomation()
{
	const float FallSpeed = 0.25f;
	const float AddDefomationValue = 0.04f;

	//変形前なら
	base_pos_.m128_f32[1] -= FallSpeed;
	//地面に着いたとき
	if (base_pos_.m128_f32[1] <= 0) {
		base_pos_.m128_f32[1] = 0;
		defomation_count_ += AddDefomationValue;
		if (headpart_scl_.z <= 0.2f && armspart_scl_.z <= 0.12f) {
			DeploymentScale();
		}
	}

	if (defomation_count_ <= 1) { return; }

	state_ = State::kMove;
}

void Low::TrackPlayerMode()
{

	float LimitLength = 1.5f;

	value_ = HelperMath::GetInstance()->TrackCalculation(base_pos_, track_point_);
	
	TrackCalculation();
	//距離をもとの値に戻す
	distance_ = origin_distance_;
	//距離の計算
	distance_ -= length_;
	//追尾移動
	HelperMath::GetInstance()->TrackEnemytoPlayer(base_pos_, TrackSpeed);

	armspart_rot_.x -= 10.f;

	if (move_state_ == Move::kNormal) {
		state_timer_ += 0.05f;
		if (state_timer_ >= 1) {
			state_timer_ = 0;
			move_state_ = Move::kJump;
		}
	}
	else {
		
		gravity_timer_ -= 0.1f;
		base_pos_.m128_f32[1] += gravity_ * gravity_timer_;
		if (gravity_timer_ <= -1) {
			gravity_timer_ = 1.f;
			base_pos_.m128_f32[1] = 0.f;
			move_state_ = Move::kNormal;
		}
	}

	if (length_ <= LimitLength) {
		player_hp_ -= 1;
		player_->SetHp(player_hp_);
		hp_ = 0;
		state_ = State::kDeath;
	}
}

void Low::Death()
{
	oldtrack_point_ = track_point_;
	if (headpart_color_.w > 0) {
		Transparentize();
		ParticleEffect();
	}
	if (obj_particle_.empty() && shadow_color_.w < 0) {
		dead_flag_ = true;
	}
}

void Low::DeploymentScale()
{
	Action::GetInstance()->EaseOut(headpart_scl_.x, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.y, 1.0f);
	Action::GetInstance()->EaseOut(headpart_scl_.z, 1.0f);

	Action::GetInstance()->EaseOut(armspart_scl_.x, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.y, 0.8f);
	Action::GetInstance()->EaseOut(armspart_scl_.z, 0.8f);
}

void Low::Transparentize()
{
	const float Subtraction = 0.1f;

	shadow_color_.w -= Subtraction;
	armspart_color_.w -= Subtraction;
	headpart_color_.w -= Subtraction;
}

void Low::HitColor()
{
	float SubValue = 0.2f;
	headpart_color_.y -= SubValue;
	headpart_color_.z -= SubValue;
}

void Low::EnemyCollsion()
{
	if (player_shot_ == true && hp_ > 0) {
		CommonCollision();
	}
}
