#include "Head.h"

void Head::Initialize()
{
	headModel = ObjModel::CreateFromOBJ("Head");
	Head = Object3d::Create();
	Head->SetModel(headModel);
}

void Head::SetPRS(XMVECTOR bodyPos)
{
	HeadPos = bodyPos;
	HeadPos.m128_f32[1] = bodyPos.m128_f32[1] + 1;
	Head->SetPosition(HeadPos);
	Head->SetRotation(HeadRot);
	Head->SetScale(HeadScl);
}

void Head::Update(bool arive,XMVECTOR bodyPos)
{
	if (arive == true) {
		//SetPRS(bodyPos);
	}
}

void Head::Draw(bool arive)
{

	Head->Draw();
	
}

void Head::Finalize()
{
	Head.reset();
	delete headModel;
}
