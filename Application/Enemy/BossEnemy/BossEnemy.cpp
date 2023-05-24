#include "BossEnemy.h"
#include"Action.h"
#include"HelperMath.h"
#include"SpriteManager.h"

using namespace DirectX;
const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.1f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };

BossEnemy::~BossEnemy()
{
	shadow_.reset();
	center_.reset();
	headpart_.reset();
	bodypart_.reset();
	armspart_.reset();
}

void BossEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint)
{
	all_pos_ = allpos;

	bringupcamera_ = camera;

	headpart_rot_ = bodypart_rot_ = armspart_rot_ = allrot;
	purse_positiverot_ += headpart_rot_.y;
	purse_negativerot_ += headpart_rot_.y;

	origin_distance_ = distance_;
	originhead_distance_ = head_distance_;

	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(2));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(2));
	headpart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(3));
	bodypart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(4));
	armspart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(5));

	partgreen_ = ParticleManager::Create(camera);
	partred_ = ParticleManager::Create(camera);

	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(all_pos_, center_mat_);

	rockon_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_pos_, rockon_color_, anchorpoint_));
	rockonhead_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockonhead_pos_, rockon_color_, anchorpoint_));

	track_point_ = oldtrack_point_ = trackpoint;

	hp_ = 800;
	oldhp_ = hp_;
	random_flag_ = true;
	timer_limit_ = 8;
	robotarive_flag_ = true;
	center_->SetPosition(center_worldpos_);

}

void BossEnemy::StatusSet()
{
	////変形前なら
	if (defomation_flag_ == false) {
		all_pos_.m128_f32[1] -= FallSpeed;
		//地面に着いたとき
		if (all_pos_.m128_f32[1] <= 2) {
			all_pos_.m128_f32[1] = 2;
			defomation_flag_ = true;
		}
	}

	center_->SetScale({ 1.0f,1.0f,1.0f });
	XMMatrixIsIdentity(center_mat_);
	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(all_pos_, center_mat_);
	center_->SetPosition(center_worldpos_);

	shadow_pos_ = center_worldpos_;
	shadow_pos_.m128_f32[1] = -0.8f;
	shadow_->SetPosition(shadow_pos_);
	shadow_->SetRotation({ 0.0f,0.0f,0.0f });
	shadow_->SetScale({ 5.0f,1.0f,5.0f });

	headpart_pos_ = armspart_pos_ = bodypart_pos_ = center_worldpos_;
	headpart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] + 4.0f;
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

	rockon_pos_ = WorldtoScreen(bodypart_pos_);
	rockonhead_pos_ = WorldtoScreen(headpart_pos_);
	rockon_->SetPosition(rockon_pos_);
	rockonhead_->SetPosition(rockonhead_pos_);
}

void BossEnemy::AllUpdate()
{
	shadow_->Update(shadow_color_);
	center_->Update();

	headpart_->Update(headpart_color_);
	bodypart_->Update(bodypart_color_);
	armspart_->Update(armspart_color_);

	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });

	for (std::unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update(50);
	}
}

void BossEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});

	//当たり判定
	if (playerbulletshot == true && hp_ > 0) {
		if (player2Dpos.x - distance_ * 4 < rockon_pos_.x && player2Dpos.x + distance_ * 4 > rockon_pos_.x &&
			player2Dpos.y - distance_ * 4 < rockon_pos_.y && player2Dpos.y + distance_ * 4 > rockon_pos_.y) {
			hp_ -= BodyDamage;
			playerbulletshot = false;
		}

		if (player2Dpos.x - head_distance_ * 4 < rockonhead_pos_.x && player2Dpos.x + head_distance_ * 4 > rockonhead_pos_.x &&
			player2Dpos.y - head_distance_ * 4 < rockonhead_pos_.y && player2Dpos.y + head_distance_ * 4 > rockonhead_pos_.y) {
			hp_ -= HeadDamage;
			playerbulletshot = false;
		}
	}

	if (Input::GetInstance()->PushKey(DIK_O)) {
		hp_ = 0;
	}

	Damage();

	Death();

	if (hp_ < 50) {
		attacktime_min_ = 15;
		attacktime_max_ = 20;
	}

	if (random_flag_ == false) {
		timer_limit_ = Action::GetInstance()->GetRangRand(attacktime_min_, attacktime_max_);
		random_flag_ = true;
	}

	//生きているとき
	if (robotarive_flag_ == true && hp_ > 0) {
		if (length_ > limit_length_ && defomation_flag_ == true) {
			TrackPlayerMode();
		}
		//プレイヤーの前まで来たとき
		else if (length_ <= limit_length_ && wait_flag_ == false) {
			bodypart_pos_.m128_f32[2] -= 1.f;
			atttack_timer_ += 0.1f;
			AttackMode(playerhp);
		}
	}
	else {
		attackfase_flag_ = false;
	}

	StatusSet();
	AllUpdate();
}

void BossEnemy::Draw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	//rockonhead_->Draw();
	//rockon_->Draw();
	Sprite::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCmdList());
	partgreen_->Draw();
	partred_->Draw();
	ParticleManager::PostDraw();

	Object3d::PreDraw(dxCommon->GetCmdList());
	for (std::unique_ptr<ObjParticle>& particle : obj_particle_) {
		particle->Draw();
	}
	if (hp_ > 0) {
		headpart_->Draw();
		bodypart_->Draw();
		armspart_->Draw();
		shadow_->Draw();
	}
	//center_->Draw();
	Object3d::PostDraw();
}

void BossEnemy::TrackPlayerMode()
{
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (all_pos_.m128_f32[0] - track_point_.m128_f32[0]);
	vy = (all_pos_.m128_f32[1] - track_point_.m128_f32[1]);
	vz = (all_pos_.m128_f32[2] - track_point_.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);
	length_ = sqrtf(v2x + v2y + v2z);

	float v3x = (vx / length_) * movespeed_;
	float v3y = (vy / length_) * movespeed_;
	float v3z = (vz / length_) * movespeed_;
	distance_ = origin_distance_;
	head_distance_ = originhead_distance_;

	distance_ -= length_ * 2.f;
	head_distance_ -= length_;

	all_pos_.m128_f32[0] -= v3x;
	all_pos_.m128_f32[2] -= v3z;
}

void BossEnemy::AttackMode(int& playerhp)
{
	if (atttack_timer_ >= timer_limit_) {
		attackfase_flag_ = true;
		random_flag_ = false;
	}
	//攻撃フェイズに移行した時
	if (attackfase_flag_ == true) {
		Action::GetInstance()->EaseOut(headpart_rot_.y, purse_positiverot_ + 1);
		if (headpart_rot_.y >= purse_positiverot_) {
			headpart_rot_.y = purse_positiverot_;
		}
		Attack(playerhp, atttack_timer_);
	}
	else {
		Action::GetInstance()->EaseOut(headpart_rot_.y, purse_negativerot_ - 1);
		if (headpart_rot_.y <= purse_negativerot_) {
			headpart_rot_.y = purse_negativerot_;
		}
	}
}

void BossEnemy::Attack(int& playerhp, float& attacktimer)
{
	//巨大化していく値
	XMFLOAT3 kGigantic = { 0.0002f ,0.0002f ,0.0002f };
	float kDiscoloration = 0.01f;
	if (attackshakedown_flag_ == false) {
		armspart_rot_.x += 1.5f;
		armspart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(armspart_scl_, kGigantic);
		bodypart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(bodypart_scl_, kGigantic);
		headpart_scl_ = HelperMath::GetInstance()->XMFLOAT3AddXMFLOAT3(headpart_scl_, kGigantic);
		armspart_color_.y -= kDiscoloration;
		armspart_color_.z -= kDiscoloration;
		//腕が最大点に達した時
		if (armspart_rot_.x >= 40.0f) {
			armspart_rot_.x = 40;
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
			attack_charge_ += 0.1f;
			if (attack_charge_ >= 10) {
				attack_charge_ = 0;
				attackshakedown_flag_ = true;
			}
		}
	}
	else {
		armspart_rot_.x -= 10.0f;
		if (armspart_rot_.x <= 0.0f) {
			armspart_rot_.x = 0.0f;
			armspart_color_ = { 1.0f,1.0f,1.0f ,1.0f };
			armspart_scl_ = { 0.2f,0.2f,0.2f };
			attackshakedown_flag_ = false;
			attackfase_flag_ = false;
			attacktimer = 0;
			playerhp -= 5;
			hp_ = 0;
			SelfDestructingEfect();
		}
	}
}

void BossEnemy::Damage()
{
	//ダメージを受けたとき
	if (oldhp_ > hp_ && hp_ >= 0) {
		oldhp_ = hp_;
		headpart_color_.y -= 0.2f;
		headpart_color_.z -= 0.2f;
		bodypart_color_.y -= 0.2f;
		bodypart_color_.z -= 0.2f;
		for (int i = 0; i < 5; i++) {
			std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
			newparticle->Initialize(1, bodypart_pos_, { 1.3f,1.3f,1.3f }, { bodypart_rot_ });
			obj_particle_.push_back(std::move(newparticle));
		}
	}
}

void BossEnemy::Death()
{
	//生きているときにHPが0になったら
	if (hp_ <= 0) {
		oldtrack_point_ = track_point_;
		notlife_flag_ = true;
		hp_ = 0;
		shadow_color_.w -= Subtraction;
		armspart_color_.w -= Subtraction;
		bodypart_color_.w -= Subtraction;
		headpart_color_.w -= Subtraction;
		if (objparticle_flag_ == true) {
			//ParticleEfect();
		}
		robotarive_flag_ = false;
		if (obj_particle_.empty()) {
			dead_flag_ = true;
		}
	}
}

XMFLOAT2 BossEnemy::WorldtoScreen(const XMVECTOR& set3Dposition)
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

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}


void BossEnemy::ParticleEfect()
{
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;

		pos.x = center_worldpos_.m128_f32[0];
		pos.y = center_worldpos_.m128_f32[1];
		pos.z = center_worldpos_.m128_f32[2];

		const float rnd_vel = 0.04f;
		XMFLOAT3 vel{};
		vel.x = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);
		vel.y = Action::GetInstance()->GetRangRand(-0.01f, 0.12f);
		vel.z = Action::GetInstance()->GetRangRand(-0.03f, 0.09f);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		partred_->Add(400, pos, vel, acc, 120.0f, 0.0f, 150.0f);
		partgreen_->Add(400, pos, vel, acc, 110.1f, 0.0f, 150.0f);
	}
	objparticle_flag_ = false;
}

void BossEnemy::SelfDestructingEfect()
{
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;

		pos.x = center_worldpos_.m128_f32[0];
		pos.y = center_worldpos_.m128_f32[1];
		pos.z = center_worldpos_.m128_f32[2];

		const float rnd_vel = 0.04f;
		XMFLOAT3 vel{};
		vel.x = Action::GetInstance()->GetRangRand(-0.09f, 0.09f);
		vel.y = Action::GetInstance()->GetRangRand(-0.01f, 0.12f);
		vel.z = Action::GetInstance()->GetRangRand(-0.03f, 0.09f);

		XMFLOAT3 acc{};
		acc.y = 0.0;

		partred_->Add(400, pos, vel, acc, 120.0f, 0.0f, 150.0f);
		partgreen_->Add(400, pos, vel, acc, 110.1f, 0.0f, 150.0f);
	}
	objparticle_flag_ = false;
}


