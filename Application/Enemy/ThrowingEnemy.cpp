#include "ThrowingEnemy.h"

void ThrowingEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera)
{
	HeadPartRot = allrot;
	BodyPartRot = allrot;
	ArmsPartRot = allrot;

	AllPos = allpos;
	BringUpCamera = camera;

	PursePositiveRot += HeadPartRot.y;
	PurseNegativeeRot += HeadPartRot.y;

	OriginDistance = Distance;
	OriginHeadDistance = HeadDistance;

	Shadow = Object3d::Create(ModelManager::GetInstance()->GetModel(2));
	Center = Object3d::Create(ModelManager::GetInstance()->GetModel(2));


	HeadPart = Object3d::Create(ModelManager::GetInstance()->GetModel(3));

	BodyPart = Object3d::Create(ModelManager::GetInstance()->GetModel(4));

	ArmsPart = Object3d::Create(ModelManager::GetInstance()->GetModel(5));

	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);

	Sprite::LoadTexture(350, L"Resources/mark.png");
	RockOn.reset(Sprite::SpriteCreate(350, RockOnPos, RockOnCol, RockOnAnchorPoint));
	RockOnHead.reset(Sprite::SpriteCreate(350, RockOnHeadPos, RockOnCol, RockOnAnchorPoint));

	HeadPartScl = { 0.0f,0.0f,0.0f };
	ArmsPartScl = { 0.0f,0.0f,0.0f };

	Hp = 160;
	OldHp = Hp;
	RobotArive = true;
	Center->SetRotation(AllRot);
	Center->SetPosition(CenterWorldPos);
}
