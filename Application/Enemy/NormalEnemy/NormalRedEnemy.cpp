#include "NormalRedEnemy.h"

const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;

void (NormalRedEnemy::* NormalRedEnemy::StateFuncTable[])() {
	&NormalRedEnemy::Defomation,
		&NormalRedEnemy::TrackMode,
		&NormalRedEnemy::WaitMode,
		&NormalRedEnemy::AttackMode,
		&NormalRedEnemy::Death,
};

NormalRedEnemy::NormalRedEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint)
{
	state_ = State::kDefomation;
	headpart_rot_ = bodypart_rot_ = armspart_rot_ = allrot;
	purse_positiverot_ += headpart_rot_.y;
	origin_distance_ = distance_;
	originhead_distance_ = head_distance_;
	headpart_scl_ = { 0.0f,0.0f,0.0f };
	armspart_scl_ = { 0.0f,0.0f,0.0f };
	track_point_ = oldtrack_point_ = trackpoint;
	base_hp_ = 160;
	old_hp_ = base_hp_;
	timer_limit_ = 8;
	bodypart_color_ = { 1.f,0.f,0.f,1.f };
	armspart_color_ = { 1.f,0.f,0.f,1.f };
}

void NormalRedEnemy::Activity()
{
	(this->*StateFuncTable[state_])();
	if (base_hp_ > 0) { return; }
	state_ = State::kDeath;
}

void NormalRedEnemy::Defomation()
{
	//•ÏŒ`‘O‚È‚ç
	base_pos_.m128_f32[1] -= FallSpeed;
	//’n–Ê‚É’…‚¢‚½‚Æ‚«
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

void NormalRedEnemy::TrackMode()
{
	TrackPlayerMode();
}

void NormalRedEnemy::WaitMode()
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

void NormalRedEnemy::AttackMode()
{
	AttackCharge();
}

void NormalRedEnemy::Death()
{
	if (headpart_color_.w >= 0) {
		Tracsparentsize();
		ParticleEfect();
	}
	if (obj_particle_.empty() && shadow_color_.w < 0) {
		dead_flag_ = true;
	}
}
