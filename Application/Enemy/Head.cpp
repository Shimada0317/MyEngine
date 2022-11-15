#include "Head.h"

Head::~Head()
{
	delete headModel;
}

void Head::Initialize()
{
	headModel = ObjModel::CreateFromOBJ("Head");
	Head = Object3d::Create(headModel);
}

void Head::SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull)
{

	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	Hit = bull->GetTrigger();
	

	HeadPos = bodyPos;
	HeadRot = rotation;
	HeadPos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(HeadPos);
	Head->SetRotation(HeadRot);
	Head->SetScale(HeadScl);
}

void Head::Updata(bool& arive,const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull,int& Hp)
{
	if (arive == true) {
		//SetPRS(bodyPos);
		SetPRS(bodyPos,rotation, bull);

		if (Collision::HeadHit(HeadPos, HeadScl, bullPos, bullScl)) {
			Hp -= 25;
			Hit = false;
			bull->SetTrigger(Hit);
			HitColor = { 1,0,0,1 };
		}
		else {
			HitColor = { 1,1,1,1 };
		}
	}
	else {
		HitColor.w -= 0.01f;
	}
	

	Head->Updata(HitColor);
}

void Head::Draw(bool arive)
{
	//if (arive == true) {
		Head->Draw();
	//}
}

void Head::Finalize()
{
	Head.reset();
	delete headModel;
}
