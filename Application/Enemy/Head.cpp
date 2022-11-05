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

void Head::SetPRS(const XMVECTOR& bodyPos,Bullet* bull)
{

	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	Hit = bull->GetTrigger();
	

	HeadPos = bodyPos;
	HeadPos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(HeadPos);
	Head->SetRotation(HeadRot);
	Head->SetScale(HeadScl);
}

void Head::Update(bool& arive,const XMVECTOR& bodyPos,Bullet* bull,int& Hp)
{
	if (arive == true) {
		//SetPRS(bodyPos);
		SetPRS(bodyPos, bull);

		if (Collision::HeadHit(HeadPos, HeadScl, bullPos, bullScl)) {
			Hp -= 25;
			Hit = false;
			bull->SetTrigger(Hit);
			HitColor = { 1,0,0,1 };
		}
	}
	else {
		HitColor = { 1,1,1,1 };
	}


	Head->Update(HitColor);
}

void Head::Draw(bool arive)
{
	if (arive == true) {
		Head->Draw();
	}
}

void Head::Finalize()
{
	Head.reset();
	delete headModel;
}
