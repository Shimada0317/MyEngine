#include "Body.h"

Body::~Body()
{
	delete BodyModel;
}

void Body::Initialize()
{
	BodyModel = ObjModel::CreateFromOBJ("tst2",true);
	Body = Object3d::Create(BodyModel);
}

void Body::SetPRS(const XMVECTOR& allPos, const XMFLOAT3 rotation, Bullet* bull)
{

	BullPos = bull->GetPosition();
	BullScl = bull->GetScl();
	Hit = bull->GetTrigger();

	Body_Pos = allPos;
	Body_Rot = rotation;

	Body->SetPosition(Body_Pos);
	Body->SetRotation(Body_Rot);
	Body->SetScale(Body_Scl);
	
}

void Body::Updata(bool& arive,const XMVECTOR& allPos, const XMFLOAT3 rotation, Bullet* bull,int& Hp)
{
	BodyMat = Body->GetMatrix();
	Body_Pos = XMVector3Transform(Body_Pos, BodyMat);
	
	if (arive == true) {
		if (BullPos.m128_f32[2] >= 10) {
			BullPos.m128_f32[2] = 0;
		}

		SetPRS(allPos ,rotation,bull);

		if (Collision::BodyHit(Body_Pos, Body_Scl, BullPos, BullScl)&&Hit==true) {
 			Hp -= 30;
			Hit = false;
			bull->SetTrigger(Hit);
			HItColor = { 1,0,0,1 };
		}
		else {
			HItColor = { 1,1,1,1 };
		}
	}
	else {
		HItColor.w -= 0.01f;
		if (HItColor.w <= 0) {
			HItColor.w = 0;
		}
	}
	
	Body->Updata(HItColor);
}

void Body::Draw(bool arive)
{
	if (HItColor.w >= 0) {
		Body->Draw();
	}
}

void Body::Finalize()
{
	Body.reset();
	delete BodyModel;
}
