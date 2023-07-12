#include "ThrowEnemy.h"
#include"Player.h"

void (ThrowEnemy::* ThrowEnemy::StateFuncTable[])() {
	& ThrowEnemy::AppearanceProcess,
	& ThrowEnemy::WaitProcess,
	& ThrowEnemy::AttackProcess,
	& ThrowEnemy::DeathProcess,
};

ThrowEnemy::ThrowEnemy(const XMFLOAT3& allrot, const XMVECTOR& allpos, const XMVECTOR& trackpoint, int type)
{
	speed_ = 0.08f;
	file = 30;
	roop_count_ = 10;
	start_scl_ = 1.f;
	end_scl_ = 0.f;
	time_ = 0.f;
	mul_value_ = 2.f;
	damage_value_ = 25;
	body_rot_ = allrot;
	body_rot_.x -= 10;
	base_pos_ = allpos;
	landing_point_ = trackpoint;
	old_hp_ = hp_;
	if (type == 1) {
		type_ = Type::kNormal;
	}
	else if (type == 2) {
		color_ = { 0.5f,0.f,0.f,1.f };
		bullet_color_ = { 0.f,0.5f,0.f,1.f };
		speed_ = 0.3f;
		add_scl_ = 0.001f;
		fall_speed_ = 0.3f;
		sub_scl_ = -0.001f;
		type_ = Type::kRed;
	}
	else if (type == 3) {
		color_ = { 0.f,0.5f,0.f,1.f };
		add_scl_ = 0.1f;
		sub_scl_ = -0.001f;
		type_ = Type::kGreen;
	}
	else if (type == 4) {
		add_scl_ = 0.001f;
		fall_speed_ = 0.5f;
		color_ = { 0.f,0.f,0.5f,1.f };
		type_ = Type::kBlue;
	}
}

void ThrowEnemy::CreateRobot(Camera* camera)
{
	
	bringupcamera_ = camera;
	//�I�u�W�F�N�g�̐���
	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	body_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kThrowEnemy));
	propeller_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kEnemyPropeller));
	bullet_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSphere));
	floating_pos_ = Action::GetInstance()->GetRangRand(6.f, 7.f);

	CommonLoad();

	StatusSet();
}

void ThrowEnemy::StatusSet()
{
	CommonStatusSet();

	body_pos_ = propeller_pos_ = center_worldpos_;

	body_->SetPosition(body_pos_);
	body_->SetRotation(body_rot_);
	body_->SetScale(body_scl_);

	bullet_->SetPosition(bullet_pos_);
	bullet_->SetRotation(bullet_rot_);
	bullet_->SetScale(bullet_scl_);

	propeller_rot_.y += 15;
	propeller_->SetPosition(propeller_pos_);
	propeller_->SetRotation(propeller_rot_);
	propeller_->SetScale(propeller_scl_);

	rockon_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, body_pos_, bringupcamera_);
	rockon_body_->SetPosition(rockon_pos_);
	rockonother_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, bullet_pos_, bringupcamera_);
	rockon_other_->SetPosition(rockonother_pos_);
}

void ThrowEnemy::AllUpdate()
{
	body_->Update(color_);
	propeller_->Update(color_);
	bullet_->Update(bullet_color_);

	CommonUpdate();
}

void ThrowEnemy::Activity(Player* player)
{
	GetPlayerState(player);

	EnemyCollision();

	(this->*StateFuncTable[state_])();

	RangeCalculation();

	Damage();

	ObjParticleDelete();

	AllUpdate();

	StatusSet();

	if (hp_ > 0) { return; }
	state_ = State::kDeath;
}

void ThrowEnemy::Draw(DirectXCommon* dxCommon)
{
	CommonParticleDraw(dxCommon);
	Object3d::PreDraw(dxCommon->GetCmdList());

	CommonObjDraw();

	body_->Draw();
	propeller_->Draw();
	if (bullet_state_ == BulletState::kShotBefore) {
		bullet_->Draw();
	}
	Object3d::PostDraw();

	//�X�v���C�g
	Sprite::PreDraw(dxCommon->GetCmdList());
	rockon_body_->Draw();
	rockon_other_->Draw();
	Sprite::PostDraw();
}

void ThrowEnemy::BulletCollision()
{
	//�e�����˂���Ă���
	if (player_shot_ == false) { return; }
	//�����蔻��
	if (!Collision::GetInstance()->CheckHit2D(player_pos_, rockonother_pos_, bullet_distance_, bullet_value_)) { return; }
	bullet_state_= BulletState::kShotAfter;
	player_shot_ = false;
}

void ThrowEnemy::ThrowAttack()
{
	value_.x = bullet_pos_.m128_f32[0] - landing_point_.m128_f32[0];
	value_.y = bullet_pos_.m128_f32[1] - landing_point_.m128_f32[1] - 2;
	value_.z = bullet_pos_.m128_f32[2] - landing_point_.m128_f32[2];
	TrackCalculation();
	bullet_distance_ = length_;
	bullet_pos_ -= TrackSpeed;
	if (type_ == Type::kRed) {
		timer_ += 0.2f;
		float DivisionValue = 2.f;
		if (timer_ >= 1) {
			timer_ = 0.f;
			float AddValue = DivisionValue / length_;
			bullet_value_ += AddValue;
		}
	}
	else {
		bullet_value_ += 0.015f;
	}
	bullet_scl_ = HelperMath::GetInstance()->XMFLOAT3AddFloat(bullet_scl_, sub_scl_);
	if (length_ <= 0.5f) {
		player_hp_ -= 1;
		player_->SetHp(player_hp_);
		bullet_pos_ = old_pos_;
		bullet_scl_ = {};
		state_ = State::kWait;
		bullet_value_ = 2.4f;
	}

	if (bullet_state_==BulletState::kShotAfter) {
		bullet_pos_ = old_pos_;
		bullet_scl_ = {};
		state_ = State::kWait;
	}
}

void ThrowEnemy::EnemyCollision()
{
	if (player_shot_ == true && hp_ > 0) {
		CommonCollision();
	}
}

void ThrowEnemy::MoveProcess()
{
	const float AbsoluteValue = 0.05f;
	const int TimerLimit = 1;
	const float AddPos = 0.1f;
	if (type_ != Type::kBlue) { return; }
	if (move_ == Move::kUp) {
		move_timer_ += AbsoluteValue;
		if (move_timer_ >= TimerLimit) {
			move_ = Move::kDown;
		}
	}
	else {
		move_timer_ -= AbsoluteValue;
		if (move_timer_ <= -TimerLimit) {
			move_ = Move::kUp;
		}
	}
	float AddValue = Action::GetInstance()->EasingOut(move_timer_, AddPos);
	base_pos_.m128_f32[1] += AddValue;
	bullet_pos_ = center_worldpos_;
	bullet_pos_.m128_f32[1] = bullet_pos_.m128_f32[1] - 1.f;
}

void ThrowEnemy::RangeCalculation()
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

	float Length = sqrtf(V2x + V2y + V2z);
	distance_ = Length;
}

void ThrowEnemy::AppearanceProcess()
{
	
	//�������Ă���
	base_pos_.m128_f32[1] -= fall_speed_;
	if (base_pos_.m128_f32[1] <= floating_pos_) {
		bullet_pos_ = center_worldpos_;
		bullet_pos_.m128_f32[1] = bullet_pos_.m128_f32[1] - 1.f;
		old_pos_ = bullet_pos_;
		state_ = State::kWait;
	}
}

void ThrowEnemy::WaitProcess()
{
	bullet_state_ = BulletState::kShotBefore;
	bullet_scl_ = HelperMath::GetInstance()->XMFLOAT3AddFloat(bullet_scl_, add_scl_);
	
	MoveProcess();
	if (bullet_scl_.z <= 0.3f) { return; }
	state_ = State::kAttack;
}

void ThrowEnemy::AttackProcess()
{
	//�e����
	ThrowAttack();
	//�e�̓����蔻��
	BulletCollision();
}

void ThrowEnemy::DeathProcess()
{
	const float kGravity = 1.8f;
	fall_time_ += 0.001f;
	float fallspeed = kGravity * fall_time_;
	color_.w -= fall_time_;
	bullet_color_.w -= fall_time_;

	if (base_pos_.m128_f32[1] >= 0) {
		base_pos_.m128_f32[1] -= fallspeed;
		if (base_pos_.m128_f32[1] <= 0) {
			dead_flag_ = true;
		}
	}
	if (color_.w <= 0) { return; }
	ParticleEffect();
}
