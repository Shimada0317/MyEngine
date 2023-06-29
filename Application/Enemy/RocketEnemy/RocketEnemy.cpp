#include "RocketEnemy.h"
#include"Action.h"
#include"Collision.h"
#include"HelperMath.h"
#include"Player.h"
#include"SpriteManager.h"

using namespace DirectX;
const int HeadDamage = 80;
const float Subtraction = 0.1f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
const float attacktime_min_ = 10;
const float attacktime_max_ = 20;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


RocketEnemy::~RocketEnemy()
{
	shadow_.reset();
	center_.reset();
	body_part_.reset();
}

void (RocketEnemy::* RocketEnemy::StateFuncTable[])() {
	&RocketEnemy::Defomation,
		&RocketEnemy::TrackPlayerMode,
		&RocketEnemy::Death,
};

void RocketEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint)
{
	state_ = State::kDefomation;
	headpart_rot_ = armspart_rot_ = allrot;

	all_pos_ = allpos;
	bringupcamera_ = camera;

	originhead_distance_ = head_distance_;

	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	body_part_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kHead));

	partgreen_ = ParticleManager::Create(bringupcamera_);
	partred_ = ParticleManager::Create(bringupcamera_);

	center_mat_ = center_->GetMatrix();
	center_worldpos_ = XMVector3TransformNormal(all_pos_, center_mat_);

	rockonhead_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockonhead_pos_, rockon_color_, anchorpoint_));

	headpart_scl_ = { 0.0f,0.0f,0.0f };
	armspart_scl_ = { 0.0f,0.0f,0.0f };

	track_point_ = oldtrack_point_ = trackpoint;
	floating_pos_ = Action::GetInstance()->GetRangRand(6.f, 7.f);
	hp_ = 160;
	oldhp_ = hp_;
	robotarive_flag_ = true;
	center_->SetPosition(center_worldpos_);
}

void RocketEnemy::StatusSet()
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

	headpart_pos_ = center_worldpos_;

	body_part_->SetPosition(headpart_pos_);
	body_part_->SetRotation(headpart_rot_);
	body_part_->SetScale(headpart_scl_);

	rockonhead_pos_ = Action::GetInstance()->WorldToScreen(center_mat_, headpart_pos_, bringupcamera_);
	rockonhead_->SetPosition(rockonhead_pos_);
}

void RocketEnemy::AllUpdate()
{
	shadow_->Update(shadow_color_);
	center_->Update();

	body_part_->Update(headpart_color_);

	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });

	for (std::unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update();
	}
}

void RocketEnemy::Update(Player* player)
{
	player_ = player;
	player_hp_ = player->GetHp();
	bool playershot = player->GetBulletShot();
	player_pos_ = player->GetRetPosition();
	if (armspart_rot_.x <= -360.f) {
		armspart_rot_.x = 0.f;
	}
	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});
	//当たり判定
	if (playershot == true && hp_ > 0) {
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

void RocketEnemy::Draw(DirectXCommon* dxCommon)
{
	ParticleManager::PreDraw(dxCommon->GetCmdList());
	partgreen_->Draw();
	partred_->Draw();
	ParticleManager::PostDraw();

	Object3d::PreDraw(dxCommon->GetCmdList());
	for (std::unique_ptr<ObjParticle>& particle : obj_particle_) {
		particle->Draw();
	}

	body_part_->Draw();
	shadow_->Draw();
	Object3d::PostDraw();
}

void RocketEnemy::Defomation()
{
	//落下してくる
	all_pos_.m128_f32[1] -= FallSpeed;
	if (all_pos_.m128_f32[1] <= floating_pos_) {
		state_ = State::kMove;
	}
}

void RocketEnemy::TrackPlayerMode()
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
	head_distance_ = originhead_distance_;
	//距離の計算
	head_distance_ -= length_;
	//追尾移動
	HelperMath::GetInstance()->TrackEnemytoPlayer(all_pos_, TrackSpeed);

	armspart_rot_.x -= 10.f;


	if (length_ <= limit_length_) {
		player_hp_ -= 1;
		player_->SetHp(player_hp_);
		hp_ = 0;
		state_ = State::kDeath;
	}
}

void RocketEnemy::Damage()
{
	//ダメージを受けたとき
	if (oldhp_ > hp_ && hp_ >= 0) {
		oldhp_ = hp_;
		HitColor();
		for (int i = 0; i < 5; i++) {
			std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
			newparticle->Initialize(1, headpart_pos_, { 0.3f,0.3f,0.3f }, { headpart_rot_ });
			obj_particle_.push_back(std::move(newparticle));
		}
	}
}

void RocketEnemy::Death()
{
	oldtrack_point_ = track_point_;
	if (headpart_color_.w > 0) {
		Transparentize();
	}
	ParticleEfect();
	robotarive_flag_ = false;
	if (obj_particle_.empty() && shadow_color_.w < 0) {
		dead_flag_ = true;
	}
}

void RocketEnemy::DeploymentScale()
{
}

void RocketEnemy::ParticleEfect()
{
	if (particleefect_flag_ != true) { return; }
	for (int i = 0; i < 50; i++) {
		XMFLOAT3 pos;
		pos = HelperMath::GetInstance()->ConvertToXMVECTOR(headpart_pos_);

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
	particleefect_flag_ = false;
}

void RocketEnemy::Transparentize()
{
	shadow_color_.w -= Subtraction;
	headpart_color_.w -= Subtraction;
}

void RocketEnemy::HitColor()
{
}

