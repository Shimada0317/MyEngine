#include "ThrowEnemy.h"
#include"ModelManager.h"

void ThrowEnemy::Initialize(const XMFLOAT3& allrot, const XMVECTOR& allpos, Camera* camera, const XMVECTOR& trackpos)
{
	HeadPart = Object3d::Create(ModelManager::GetInstance()->GetModel(1));
	BodyPart = Object3d::Create(ModelManager::GetInstance()->GetModel(1));
	BothArmsPart = Object3d::Create(ModelManager::GetInstance()->GetModel(1));

	AllPos = allpos;
	CenterMat = Center->GetMatrix();
	CenterWorldPos = XMVector3TransformNormal(AllPos, CenterMat);

	
	OldHp = Hp;
	Center->SetPosition(CenterWorldPos);
}

void ThrowEnemy::Update(const XMFLOAT2& player2Dpos, int& playerhp, bool& playerbulletshot)
{
}
