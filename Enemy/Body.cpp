#include "Body.h"

void Body::Initialize()
{
	bodyModel = ObjModel::CreateFromOBJ("body");
	body = Object3d::Create();
	body->SetModel(bodyModel);

}

void Body::SetPRS(XMVECTOR allPos,Bullet* bull)
{

	BullPos = bull->GetPosition();
	BullScl = bull->GetScl();

	BodyPos = allPos;
	body->SetPosition(BodyPos);
	body->SetRotation(BodyRot);
	body->SetScale(BodyScl);
}

void Body::Update(bool& arive,XMVECTOR allPos,Bullet* bull)
{

	if (arive == true) {
	}

	if (BullPos.m128_f32[2] >= 10) {
		BullPos.m128_f32[2] = 0;
	}

	SetPRS(allPos,bull);

	body->Update();

	

	if (Collision::BodyHit(BodyPos, BodyScl, BullPos, BullScl)) {
		arive = false;
	}
}

void Body::Draw(bool& arive)
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
