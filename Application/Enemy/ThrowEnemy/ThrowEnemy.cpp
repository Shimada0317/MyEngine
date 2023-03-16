#include "ThrowEnemy.h"
#include"Action.h"
#include"ModelManager.h"

using namespace DirectX;

const int HeadDamage = 80;
const int BodyDamage = 50;
const float Subtraction = 0.05f;
const float FallSpeed = 0.25f;
const float AddDefomationValue = 0.04f;
const XMFLOAT4 AddColor = { 0.1f,0.1f,0.1f,0.0f };


ThrowEnemy::~ThrowEnemy()
{
	HeadPart.reset();
	BodyPart.reset();
	ArmsPart.reset();
	Shadow.reset();
	Center.reset();
}

void ThrowEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos)
{
	HeadPart = Object3d::Create(ModelManager::GetInstance()->GetModel(3));
	BodyPart = Object3d::Create(ModelManager::GetInstance()->GetModel(4));
	ArmsPart = Object3d::Create(ModelManager::GetInstance()->GetModel(5));
	Center= Object3d::Create(ModelManager::GetInstance()->GetModel(2));
	Shadow = Object3d::Create(ModelManager::GetInstance()->GetModel(2));

	HeadRot = BodyRot = ArmsRot = allrot;
	AllPos = allpos;
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);

	OriginDistance = Distance;
	OriginHeadDistance = HeadDistance;
	OriginBoxDistance = BoxDistance;

	LandingPoint = trackpos;
	OldHp = Hp;
	Center->SetPosition(CenterWorldPos);
}

void ThrowEnemy::StatusSet()
{

	//•ÏŒ`‘O‚È‚ç
	if (DefomationFlag == false) {
		AllPos.m128_f32[1] -= FallSpeed;
		//’n–Ê‚É’…‚¢‚½‚Æ‚«
		if (AllPos.m128_f32[1] <= 0) {
			AllPos.m128_f32[1] = 0;
			DefomationCount += AddDefomationValue;
			if (HeadScl.z <= 0.3f && ArmsScl.z <= 0.2f) {
				Action::GetInstance()->EaseOut(HeadScl.x, 1.0f);
				Action::GetInstance()->EaseOut(HeadScl.y, 1.0f);
				Action::GetInstance()->EaseOut(HeadScl.z, 1.0f);

				Action::GetInstance()->EaseOut(ArmsScl.x, 0.8f);
				Action::GetInstance()->EaseOut(ArmsScl.y, 0.8f);
				Action::GetInstance()->EaseOut(ArmsScl.z, 0.8f);
			}
		}
	}

	if (DefomationCount >= 1) {
		DefomationCount = 1;
		DefomationFlag = true;
	}

	Center->SetScale({ 1.f,1.f,1.f });

	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);
	Center->SetPosition(CenterWorldPos);

	ShadowPos = CenterWorldPos;
	ShadowPos.m128_f32[1] = -0.8f;
	Shadow->SetPosition(ShadowPos);
	
	HeadPos = ArmsPos = BodyPos =ThrowBoxPos = CenterWorldPos;
	HeadPos.m128_f32[1] = CenterWorldPos.m128_f32[1] + 1.0f;
	ArmsPos.m128_f32[1] = CenterWorldPos.m128_f32[1] + 0.2f;

	HeadPart->SetPosition(HeadPos);
	HeadPart->SetScale(HeadScl);
	HeadPart->SetRotation(HeadRot);

	BodyPart->SetPosition(BodyPos);
	BodyPart->SetScale(BodyScl);
	BodyPart->SetRotation(BodyRot);

	ArmsPart->SetPosition(ArmsPos);
	ArmsPart->SetScale(ArmsScl);
	ArmsPart->SetRotation(ArmsRot);

	/*ThrowBox->SetPosition(ThrowBoxPos);
	ThrowBox->SetRotation(ThrowBoxRot);
	ThrowBox->SetScale(ThrowBoxScl);*/
}

void ThrowEnemy::AllUpdate()
{
	Shadow->Update(ShadowCol);
	HeadPart->Update();
	BodyPart->Update();
	ArmsPart->Update();
	//ThrowBox->Update();
	Center->Update();
}

void ThrowEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{

	StatusSet();
	AllUpdate();
}

void ThrowEnemy::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	HeadPart->Draw();
	BodyPart->Draw();
	ArmsPart->Draw();
	Shadow->Draw();
	//ThrowBox->Draw();
	Object3d::PostDraw();
}

void ThrowEnemy::ThrowAttack()
{
	float vx = 0;
	float vy = 0;
	float vz = 0;

	vx = (AllPos.m128_f32[0] - LandingPoint.m128_f32[0]);
	vy = (AllPos.m128_f32[1] - LandingPoint.m128_f32[1]);
	vz = (AllPos.m128_f32[2] - LandingPoint.m128_f32[2]);

	float v2x = pow(vx, 2);
	float v2y = pow(vy, 2);
	float v2z = pow(vz, 2);

	Length = sqrtf(v2x + v2y + v2z);

	XMVECTOR v3;
	v3.m128_f32[0] = (vx / Length) * MoveSpeed;
	v3.m128_f32[1] = (vy / Length) * MoveSpeed;
	v3.m128_f32[2] = (vz / Length) * MoveSpeed;
	Distance = OriginDistance;
	HeadDistance = OriginHeadDistance;
	BoxDistance = OriginBoxDistance;

	Distance -= Length * 2.0f;
	HeadDistance -= Length;
	BoxDistance -= Length *2.f;

	Action::GetInstance()->XMvectorSubXMVector(AllPos,v3);
}
