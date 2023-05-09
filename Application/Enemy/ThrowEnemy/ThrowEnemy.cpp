#include "ThrowEnemy.h"
#include"Action.h"
#include"HelperMath.h"
#include"ModelManager.h"
#include"SpriteManager.h"

using namespace DirectX;

const int BodyDamage = 25;
const float FallSpeed = 0.15f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


ThrowEnemy::~ThrowEnemy()
{
	enemy_.reset();
	propeller_.reset();
}

void ThrowEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos)
{

	enemy_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kThrowEnemy));
	propeller_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kEnemyPropeller));
	Center = Object3d::Create(ModelManager::GetInstance()->GetModel(kShadow));
	bullet_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kSphere));

	body_rot_ = allrot;
	body_rot_.x -= 10;
	center_pos_ = allpos;
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(center_pos_, CenterMat);
	HaveCamera = camera;
	RockOn.reset(Sprite::SpriteCreate(Name::kEnemyMarker, RockOnPos, RockOnCol, RockOnAnchorPoint));
	rockon_bullet_.reset(Sprite::SpriteCreate(Name::kEnemyMarker, rockon_bulletpos_, RockOnCol, RockOnAnchorPoint));

	floating_pos_ = Action::GetInstance()->GetRangRand(6.f, 7.f);

	LandingPoint = trackpos;
	OldHp = Hp;
	Center->SetPosition(CenterWorldPos);
}

void ThrowEnemy::StatusSet()
{

	Center->SetScale({ 1.f,1.f,1.f });
	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(center_pos_, CenterMat);
	Center->SetPosition(CenterWorldPos);

	body_pos_ = propeller_pos_ = CenterWorldPos;

	enemy_->SetPosition(body_pos_);
	enemy_->SetRotation(body_rot_);
	enemy_->SetScale(body_scl_);
	
	bullet_->SetPosition(bullet_pos_);
	bullet_->SetRotation(bullet_rot_);
	bullet_->SetScale(bullet_scl_);

	propeller_rot_.y += 15;
	propeller_->SetPosition(propeller_pos_);
	propeller_->SetRotation(propeller_rot_);
	propeller_->SetScale(propeller_scl_);

	RockOnPos = WorldtoScreen(body_pos_);
	RockOn->SetPosition(RockOnPos);
	rockon_bulletpos_ = WorldtoScreen(bullet_pos_);
	rockon_bullet_->SetPosition(rockon_bulletpos_);
}

void ThrowEnemy::AllUpdate()
{
	enemy_->Update();
	propeller_->Update();
	Center->Update();
	bullet_->Update();
}

void ThrowEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	AppearanceProcess();

	WaitProcess();

	AttackProcess(player2Dpos,playerbulletshot);

	DamageProcess(player2Dpos,playerbulletshot);

	DeathProcess();

	

	if (Input::GetInstance()->PushKey(DIK_O)) {
		Hp = 0;
	}
	if (Hp <= 0) {
		state_ = State::DEATH;
	}

	if (state_ == State::DEATH) {
		
	}

	StatusSet();
	AllUpdate();
}

void ThrowEnemy::AppearanceProcess()
{
	if (state_ != State::APPEARANCE) { return; }
	center_pos_.m128_f32[1] -= FallSpeed;
	if (center_pos_.m128_f32[1] <= floating_pos_) {
		bullet_pos_ = CenterWorldPos;
		bullet_pos_.m128_f32[1] =bullet_pos_.m128_f32[1] - 1.f;
		old_pos_ = bullet_pos_;
		state_ = State::WAIT;
	}

}

void ThrowEnemy::WaitProcess()
{
	if (state_ != State::WAIT) { return; }
	bullet_active_ = true;
	bullet_scl_=HelperMath::GetInstance()->XMFLOAT3AddFloat(bullet_scl_, 0.001f);
	if (bullet_scl_.z <= 0.3f) { return; }
	state_ = State::ATTACK;
}

void ThrowEnemy::AttackProcess(const XMFLOAT2& player2Dpos, bool& playerbulletshot)
{
	if (state_ != State::ATTACK) { return; }
	ThrowAttack();
	BulletCollision(player2Dpos, playerbulletshot);
}

void ThrowEnemy::DamageProcess(const XMFLOAT2& player2Dpos, bool& playerbulletshot)
{
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (bullet_pos_.m128_f32[0] - LandingPoint.m128_f32[0]);
	vy = (bullet_pos_.m128_f32[1] - LandingPoint.m128_f32[1] - 2);
	vz = (bullet_pos_.m128_f32[2] - LandingPoint.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);

	Length = sqrtf(v2x + v2y + v2z);
	Distance = Length;

	//“–‚½‚è”»’è
	if (playerbulletshot == true && Hp > 0) {
		if (player2Dpos.x - Distance * 2.f < RockOnPos.x && player2Dpos.x + Distance * 2.f > RockOnPos.x &&
			player2Dpos.y - Distance * 2.f < RockOnPos.y && player2Dpos.y + Distance * 2.f > RockOnPos.y) {
			Hp -= BodyDamage;
			playerbulletshot = false;
		}
	}
}

void ThrowEnemy::DeathProcess()
{
	if (state_ != State::DEATH) { return; }
	center_pos_.m128_f32[1] -= 0.1f;
	if (center_pos_.m128_f32[1] <= 0) {
		DeadFlag = true;
	}
}

XMFLOAT2 ThrowEnemy::WorldtoScreen(const XMVECTOR& set3Dposition)
{
	Center->SetRotation(CenterRot);
	CenterMat = Center->GetMatrix();
	const float kDistancePlayerTo3DReticle = 50.0f;
	offset = { 0.0,0.0,1.0f };
	offset = XMVector3TransformNormal(offset, CenterMat);
	offset = XMVector3Normalize(offset) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = set3Dposition;

	HelperMath::GetInstance()->ChangeViewPort(MatViewPort, offset);

	XMMATRIX MatVP = MatViewPort;

	XMMATRIX View = HaveCamera->GetViewMatrix();
	XMMATRIX Pro = HaveCamera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}

void ThrowEnemy::Draw(DirectXCommon* dxCommon)
{
	
	Object3d::PreDraw(dxCommon->GetCmdList());
	enemy_->Draw();
	propeller_->Draw();
	if (bullet_active_==true) {
		bullet_->Draw();
	}
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	RockOn->Draw();
	rockon_bullet_->Draw();
	Sprite::PostDraw();
}

void ThrowEnemy::BulletCollision(const XMFLOAT2& player2Dpos, bool& playerbulletshot)
{
	//“–‚½‚è”»’è
	if (playerbulletshot == false) { return; }
	if (player2Dpos.x - bullet_distance_ * 2.f < rockon_bulletpos_.x && player2Dpos.x + bullet_distance_ * 2.f > rockon_bulletpos_.x &&
		player2Dpos.y - bullet_distance_ * 2.f < rockon_bulletpos_.y && player2Dpos.y + bullet_distance_ * 2.f > rockon_bulletpos_.y) {
		bullet_active_ = false;
		playerbulletshot = false;
	}
}

void ThrowEnemy::ThrowAttack()
{
	float vx = 0;
	float vy = 0;
	float vz = 0;
	float bullet_length = 0;

	vx = (bullet_pos_.m128_f32[0] - LandingPoint.m128_f32[0]);
	vy = (bullet_pos_.m128_f32[1] - LandingPoint.m128_f32[1]-2);
	vz = (bullet_pos_.m128_f32[2] - LandingPoint.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);

	bullet_length = sqrtf(v2x + v2y + v2z);

	XMVECTOR v3;
	v3.m128_f32[0] = (vx / bullet_length) * bullet_speed_;
	v3.m128_f32[1] = (vy / bullet_length) * bullet_speed_;
	v3.m128_f32[2] = (vz / bullet_length) * bullet_speed_;

	
	bullet_distance_ = bullet_length;

	bullet_pos_ -= v3;
	if (bullet_length <=0.1f||bullet_active_==false) {
		bullet_pos_ = old_pos_;
		bullet_scl_={};
		state_ = State::WAIT;
	}
}
