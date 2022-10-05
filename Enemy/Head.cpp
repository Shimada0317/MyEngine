#include "Head.h"

void Head::Initialize()
{
	headModel = ObjModel::CreateFromOBJ("Head");
	Head = Object3d::Create();
	Head->SetModel(headModel);
	
}

void Head::SetPRS(XMVECTOR bodyPos,Bullet* bull)
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

void Head::Update(bool& arive,XMVECTOR bodyPos,Bullet* bull,int& Hp)
{
	if (arive == true) {
		//SetPRS(bodyPos);
		SetPRS(bodyPos, bull);

		if (Collision::HeadHit(HeadPos, HeadScl, bullPos, bullScl)) {
			Hp -= 25;
			Hit = false;
			bull->SetTrigger(Hit);
		}
	}


	Head->Update();
}

void Head::Draw(bool& arive)
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
