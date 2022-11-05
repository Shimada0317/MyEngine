#include "Body.h"

Body::~Body()
{
	delete bodyModel;
}

void Body::Initialize()
{
	bodyModel = ObjModel::CreateFromOBJ("body");
	body = Object3d::Create(bodyModel);
}

void Body::SetPRS(const XMVECTOR& allPos,Bullet* bull)
{

	BullPos = bull->GetPosition();
	BullScl = bull->GetScl();
	Hit = bull->GetTrigger();

	BodyPos = allPos;
	body->SetPosition(BodyPos);
	body->SetRotation(BodyRot);
	body->SetScale(BodyScl);
}

void Body::Update(bool& arive,const XMVECTOR& allPos,Bullet* bull,int& Hp)
{
	Hit = bull->GetTrigger();
	if (arive == true) {


		if (BullPos.m128_f32[2] >= 10) {
			BullPos.m128_f32[2] = 0;
		}

		SetPRS(allPos, bull);

		if (Collision::BodyHit(BodyPos, BodyScl, BullPos, BullScl)&&Hit==true) {
			Hp -= 10;
			Hit = false;
			bull->SetTrigger(Hit);
			HItColor = { 1,0,0,1 };
		}
	}
	else {
		HItColor = { 1,1,1,1 };
	}
	body->Update(HItColor);
}

void Body::Draw(bool arive)
{
	if (arive == true) {
		body->Draw();
	}
}

void Body::Finalize()
{
	body.reset();
	delete bodyModel;
}
