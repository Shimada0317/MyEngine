#include "ThrowEnemy.h"
#include"Action.h"
#include"HelperMath.h"
#include"ModelManager.h"
#include"SpriteManager.h"

using namespace DirectX;

const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.05f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


ThrowEnemy::~ThrowEnemy()
{
	enemy_.reset();
	propeller_.reset();
}

void ThrowEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos)
{
	
	enemy_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kThrowEnemy));
	propeller_ = Object3d::Create(ModelManager::GetInstance()->GetModel(kHane));

	body_rot_ = allrot;
	body_pos_=center_pos_ = allpos;
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(center_pos_, CenterMat);

	RockOn.reset(Sprite::SpriteCreate(Name::kEnemyMarker, RockOnPos, RockOnCol, RockOnAnchorPoint));

	floating_pos_ - Action::GetInstance()->GetRangRand(2.f, 4.f);

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

	ShadowPos = CenterWorldPos;
	ShadowPos.m128_f32[1] = -0.8f;
	Shadow->SetPosition(ShadowPos);
	
	body_pos_ = propeller_pos_ = CenterWorldPos;

	enemy_->SetPosition(body_pos_);
	enemy_->SetRotation(body_rot_);
	enemy_->SetScale(body_scl_);

	propeller_->SetPosition(propeller_pos_);
	propeller_->SetRotation(propeller_rot_);
	propeller_->SetScale(propeller_scl_);


}

void ThrowEnemy::AllUpdate()
{
	Shadow->Update(ShadowCol);
	enemy_->Update();
	propeller_->Update();
	//ThrowBox->Update();
	Center->Update();
}

void ThrowEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
	//“oêˆ—
	AppearanceProcess();
	//UŒ‚ˆ—
	AttackProcess();

	StatusSet();
	AllUpdate();
}

void ThrowEnemy::AppearanceProcess()
{
	if (state_ != APPEARANCE) { return; }
	if (center_pos_.m128_f32[1] <= floating_pos_) {
		center_pos_.m128_f32[1] -= FallSpeed;
	}
	else {
		state_ = WAIT;
	}
}

void ThrowEnemy::AttackProcess()
{
	if (state_ != ATTACK) { return; }
	ThrowAttack();
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
	//ThrowBox->Draw();
	Object3d::PostDraw();
}

void ThrowEnemy::ThrowAttack()
{
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (ThrowBoxPos.m128_f32[0] - LandingPoint.m128_f32[0]);
	vy = (ThrowBoxPos.m128_f32[1] - LandingPoint.m128_f32[1]);
	vz = (ThrowBoxPos.m128_f32[2] - LandingPoint.m128_f32[2]);

	float v2x = powf(vx, 2.f);
	float v2y = powf(vy, 2.f);
	float v2z = powf(vz, 2.f);

	Length = sqrtf(v2x + v2y + v2z);

	XMVECTOR v3;
	v3.m128_f32[0] = (vx / Length) * bullet_speed_;
	v3.m128_f32[1] = (vy / Length) * bullet_speed_;
	v3.m128_f32[2] = (vz / Length) * bullet_speed_;

	Distance -= Length * 2.0f;
	HeadDistance -= Length;
	BoxDistance -= Length *2.f;

	center_pos_=HelperMath::GetInstance()->XMvectorSubXMVector(center_pos_,v3);
}
