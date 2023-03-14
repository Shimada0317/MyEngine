#include "ThrowEnemy.h"
#include"ModelManager.h"

using namespace DirectX;


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
	HeadPart = Object3d::Create(ModelManager::GetInstance()->GetModel(1));
	BodyPart = Object3d::Create(ModelManager::GetInstance()->GetModel(1));
	ArmsPart = Object3d::Create(ModelManager::GetInstance()->GetModel(1));

	AllPos = allpos;
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);

	
	OldHp = Hp;
	Center->SetPosition(CenterWorldPos);
}

void ThrowEnemy::StatusSet()
{

	Center->SetScale({ 1.f,1.f,1.f });

	XMMatrixIsIdentity(CenterMat);
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3Transform(AllPos, CenterMat);
	Center->SetPosition(CenterWorldPos);

	ShadowPos = CenterWorldPos;
	ShadowPos.m128_f32[1] = -0.8f;
	Shadow->SetPosition(ShadowPos);
	
	HeadPos = ArmsPos = BodyPos = CenterWorldPos;
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
}

void ThrowEnemy::AllUpdate()
{
	Shadow->Update();
	HeadPart->Update();
	BodyPart->Update();
	ArmsPart->Update();
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
	Object3d::PostDraw();
}
