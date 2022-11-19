#include "Body.h"

Body::~Body()
{
	delete bodyModel;
}

void Body::Initialize()
{
	bodyModel = ObjModel::CreateFromOBJ("tst2",true);
	body = Object3d::Create(bodyModel);
}

void Body::SetPRS(const XMVECTOR& allPos, const XMFLOAT3 rotation, Bullet* bull)
{

	BullPos = bull->GetPosition();
	BullScl = bull->GetScl();
	Hit = bull->GetTrigger();


	BodyPos = allPos;
	BodyRot = rotation;

	body->SetPosition(BodyPos);
	body->SetRotation(BodyRot);
	body->SetScale(BodyScl);
	BodyMat=body->GetMatrix();
}

void Body::Updata(bool& arive,const XMVECTOR& allPos, const XMFLOAT3 rotation, Bullet* bull,int& Hp)
{
	
	BodyPos = XMVector3Transform(BodyPos, BodyMat);

	Hit = bull->GetTrigger();
	if (arive == true) {


		if (BullPos.m128_f32[2] >= 10) {
			BullPos.m128_f32[2] = 0;
		}

		SetPRS(allPos ,rotation,bull);

		if (Collision::BodyHit(BodyPos, BodyScl, BullPos, BullScl)&&Hit==true) {
 			Hp -= 20;
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
		
	}
	
	body->Updata(HItColor);
}

void Body::Draw(bool arive)
{
	if (HItColor.w >= 0) {
		body->Draw();
	}

}

void Body::Finalize()
{
	body.reset();
	delete bodyModel;
}
