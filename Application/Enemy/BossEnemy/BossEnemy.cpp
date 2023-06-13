#include "BossEnemy.h"
#include"Action.h"
#include"Camera.h"
#include"Collision.h"
#include"HelperMath.h"
#include"SpriteManager.h"


using namespace DirectX;
const int HeadDamage = 80;
const int BodyDamage = 50;
const int Coredamage = 25;
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
}

void BossEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpoint)
{
	//中心座標
	all_pos_ = allpos;
	//カメラ
	bringupcamera_ = camera;
	//角度を共通化
	headpart_rot_ = bodypart_rot_ = corepart_rot_ = allrot;
	origin_distance_ = distance_;
	originhead_distance_ = head_distance_;
	origincore_distance_ = core_distance_;
	//オブジェクトの生成
	shadow_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	center_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	headpart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kHead));
	bodypart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kBossBody));
	corepart_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kBossCore));
	//パーティクル生成
	partgreen_ = ParticleManager::Create(bringupcamera_);
	partred_ = ParticleManager::Create(bringupcamera_);
	//敵の2D座標
	rockon_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_pos_, rockon_color_, anchorpoint_));
	rockonhead_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockonhead_pos_, rockon_color_, anchorpoint_));
	rockoncore_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockonhead_pos_, rockon_color_, anchorpoint_));
	track_point_ = trackpoint;
}

void BossEnemy::StatusSet()
{
	if (center_worldpos_.m128_f32[1] >= 10) {
		headpart_scl_ = { 1.f,1.f,1.f };
		bodypart_scl_ = { 1.4f,1.4f,1.4f };
		corepart_scl_ = { 0.8f,0.8f,0.8f };

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
	shadow_->SetScale({ 2.0f,1.0f,2.0f });

	headpart_pos_ = bodypart_pos_ = corepart_pos_ = center_worldpos_;
	bodypart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] - 1.f;
	corepart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] - 3.f;
	headpart_pos_.m128_f32[1] = center_worldpos_.m128_f32[1] + 4.0f;

	headpart_->SetPosition(headpart_pos_);
	headpart_->SetRotation(headpart_rot_);
	headpart_->SetScale(headpart_scl_);

	bodypart_->SetPosition(bodypart_pos_);
	bodypart_->SetRotation(bodypart_rot_);
	bodypart_->SetScale(bodypart_scl_);

	corepart_->SetPosition(corepart_pos_);
	corepart_->SetRotation(corepart_rot_);
	corepart_->SetScale(corepart_scl_);

	rockon_pos_ = WorldtoScreen(bodypart_pos_);
	rockonhead_pos_ = WorldtoScreen(headpart_pos_);
	rockoncore_pos_ = WorldtoScreen(corepart_pos_);
	rockon_->SetPosition(rockon_pos_);
	rockonhead_->SetPosition(rockonhead_pos_);
	rockoncore_->SetPosition(rockoncore_pos_);
}

void BossEnemy::AllUpdate()
{
	shadow_->Update(shadow_color_);
	center_->Update();

	headpart_->Update(headpart_color_);
	bodypart_->Update(bodypart_color_);
	corepart_->Update(corepart_color_);

	partred_->Update({ 1.0f,0.0f,0.0f,0.0f });
	partgreen_->Update({ 0.0f,0.5f,0,0.0f });

	for (std::unique_ptr<ObjParticle>& patrticle : obj_particle_) {
		patrticle->Update(50);
	}
}

void BossEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	Appearance();

	Move(player2Dpos, playerhp, playerbulletshot);

	Stun(player2Dpos, playerhp, playerbulletshot);

	obj_particle_.remove_if([](std::unique_ptr<ObjParticle>& particle) {
		return particle->IsDelete();
		});

	//当たり判定
	if (playerbulletshot == true && hp_ > 0) {
		if (Collision::GetInstance()->CheckHit2D(player2Dpos, rockoncore_pos_, core_distance_, 4)) {
			hp_ -= Coredamage;
			addrot_ -= 5.f;
			playerbulletshot = false;
		}
	}

	Damage();

	Death();

	Attack(playerhp);

	StatusSet();

	AllUpdate();
}

void BossEnemy::Draw(DirectXCommon* dxCommon)
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
	corepart_->Draw();
	shadow_->Draw();

	Object3d::PostDraw();
}

void BossEnemy::Appearance()
{
	if (state_ != kAppearance) { return; }
	all_pos_.m128_f32[1] -= FallSpeed;
	if (all_pos_.m128_f32[1] <= 3.f) {
		all_pos_.m128_f32[1] = 3.f;
		state_ = kMove;
	}
}

void BossEnemy::Move(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	if (state_ != kMove) { return; }
	float endcolor_ = 0.f;
	float startcolor_ = 1.f;
	Action::GetInstance()->LoopTimer(color_time_, 0.1f, 50);
	corepart_color_.y = Action::GetInstance()->EasingOut(color_time_, endcolor_ - startcolor_);
	corepart_color_.z = Action::GetInstance()->EasingOut(color_time_, endcolor_ - startcolor_);
	corepart_rot_.y += addrot_;
	bodypart_rot_.y += addrot_;
	headpart_rot_.y += addrot_;
	if (length_ > limit_length_) {
		TrackPlayerMode();
	}
	else {
		state_ = kAttack;
	}
	if (addrot_ <= 0) {
		state_ = kStun;
		all_pos_.m128_f32[2] -= movespeed_ * 2;
	}
}

void BossEnemy::Stun(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	if (state_ != kStun) { return; }
	movespeed_ = 0.f;
	addrot_ = 0.f;
	all_pos_.m128_f32[2] -= 0.01f;
	if (playerbulletshot == true && hp_ > 0) {
		if (Collision::GetInstance()->CheckHit2D(player2Dpos,rockonhead_pos_,head_distance_,4)) {
			hp_ -= HeadDamage;
			playerbulletshot = false;
		}

		if (Collision::GetInstance()->CheckHit2D(player2Dpos, rockonhead_pos_, distance_, 4)) {
			hp_ -= BodyDamage;
			playerbulletshot = false;
		}
	}
	time_ += 0.1f;
	if (time_ >= 10) {
		state_ = kMove;
		addrot_ = 20;
		movespeed_ = 0.02f;
		time_ = 0.f;
	}
}

void BossEnemy::TrackPlayerMode()
{
	//追尾の計算
	XMFLOAT3 Value;
	Value = HelperMath::GetInstance()->TrackCalculation(all_pos_, track_point_);
	//値を2乗
	XMFLOAT3 SquareValue{};
	SquareValue = HelperMath::GetInstance()->SquareToXMFLOAT3(Value,2);
	//距離の計算
	length_ = HelperMath::GetInstance()->LengthCalculation(SquareValue);
	//追尾速度の計算
	XMVECTOR TrackSpeed{};
	TrackSpeed = HelperMath::GetInstance()->TrackingVelocityCalculation(Value, length_, movespeed_);
	distance_ = origin_distance_;
	head_distance_ = originhead_distance_;
	core_distance_ = origincore_distance_;

	distance_ -= length_ * 2.f;
	head_distance_ -= length_;
	core_distance_ -= length_;

	HelperMath::GetInstance()->TrackEnemytoPlayer(all_pos_,TrackSpeed);
}

void BossEnemy::Damage()
{
	//ダメージを受けたとき
	if (oldhp_ > hp_ && hp_ >= 0) {
		oldhp_ = hp_;
		headpart_color_.y -= 0.2f;
		headpart_color_.z -= 0.2f;
		for (int i = 0; i < 5; i++) {
			std::unique_ptr<ObjParticle> newparticle = std::make_unique<ObjParticle>();
			newparticle->Initialize(1, bodypart_pos_, { 1.3f,1.3f,1.3f }, { bodypart_rot_ });
			obj_particle_.push_back(std::move(newparticle));
		}
	}

	if (hp_ > 0) { return; }
	state_ = State::kDead;

}

void BossEnemy::Death()
{
	if (state_ != State::kDead) { return; }
	//生きているときにHPが0になったら
	shadow_color_.w -= Subtraction;
	bodypart_color_.w -= Subtraction;
	headpart_color_.w -= Subtraction;
	corepart_color_.w -= Subtraction;

	robotarive_flag_ = false;
	if (obj_particle_.empty()) {
		dead_flag_ = true;
	}

	if (bodypart_color_.w <= 0.5f) { return; }
	ParticleEfect();
}

void BossEnemy::Attack(int& playerhp)
{
	if (state_ != kAttack) { return; }
	attack_timer_ += 0.1f;
	addrot_ = 100.f;
	corepart_rot_.y += addrot_;
	bodypart_rot_.y += addrot_;
	headpart_rot_.y += addrot_;
	attack_speed_ += 0.1f;
	float Rand = Action::GetInstance()->GetRangRand(-0.05f, 0.05f);
	all_pos_.m128_f32[0] += Rand;
	if (attack_timer_ < 4) { return; }
	all_pos_.m128_f32[2] += attack_speed_;
	if (attack_speed_ > 0.5f) {
		attack_speed_ = 0.f;
		playerhp -= 5;
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

	for (int i = 0; i < 5; i++) {
		XMFLOAT3 pos;

		pos.x = bodypart_pos_.m128_f32[0];
		pos.y = bodypart_pos_.m128_f32[1];
		pos.z = bodypart_pos_.m128_f32[2];

		const float rnd_vel = 0.04f;
		XMFLOAT3 Vel{};
		XMFLOAT3 MinValue{ -0.09f,-0.01f,-0.03f };
		XMFLOAT3 MaxValue{ 0.09f,0.12f,0.09f };
		Vel = Action::GetInstance()->RandMinAndMax(MinValue, MaxValue);

		XMFLOAT3 Acc{};

		partred_->Add(200, pos, Vel, Acc, 100.f, 0.f, 150.f);
		partgreen_->Add(200, pos, Vel, Acc, 90.f, 0.f, 150.f);
	}
	particleefect_flag_ = false;

}

