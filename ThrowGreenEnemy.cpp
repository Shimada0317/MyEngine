#include "ThrowGreenEnemy.h"

void (ThrowGreenEnemy::* ThrowGreenEnemy::StateFuncTable[])() {
	&ThrowGreenEnemy::AppearanceProcess,
		& ThrowGreenEnemy::WaitProcess,
		& ThrowGreenEnemy::AttackProcess,
		& ThrowGreenEnemy::DeathProcess,
};

ThrowGreenEnemy::ThrowGreenEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpos)
{
	color_ = { 0.f,1.f,0.f,1.f };
	floating_pos_ = Action::GetInstance()->GetRangRand(6.f, 7.f);
	body_rot_ = allrot;
	body_rot_.x -= 10;
	center_pos_ = allpos;
	landing_point_ = trackpos;
	oldhp_ = hp_;

}

void ThrowGreenEnemy::Activity()
{
	//関数ポインタで状態遷移
	(this->*StateFuncTable[state_])();
	if (hp_ > 0) { return; }
	state_ = State::DEATH;
}

void ThrowGreenEnemy::AppearanceProcess()
{
	const float FallSpeed = 0.15f;
	//落下してくる
	center_pos_.m128_f32[1] -= FallSpeed;
	if (center_pos_.m128_f32[1] <= floating_pos_) {
		bullet_pos_ = center_worldpos_;
		bullet_pos_.m128_f32[1] = bullet_pos_.m128_f32[1] - 1.f;
		old_pos_ = bullet_pos_;
		state_ = State::WAIT;
	}
}

void ThrowGreenEnemy::WaitProcess()
{
	bullet_active_ = true;
	bullet_scl_ = HelperMath::GetInstance()->XMFLOAT3AddFloat(bullet_scl_, 0.005f);
	if (bullet_scl_.z <= 0.3f) { return; }
	state_ = State::ATTACK;
}

void ThrowGreenEnemy::AttackProcess()
{
	//弾発射
	ThrowAttack();
	//弾の当たり判定
	BulletCollision();
}

void ThrowGreenEnemy::DeathProcess()
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
