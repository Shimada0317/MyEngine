#include "Body.h"

void Body::Initialize()
{
	bodyModel = ObjModel::CreateFromOBJ("body");
	body = Object3d::Create();
	body->SetModel(bodyModel);

}

void Body::SetPRS(XMVECTOR allPos)
{
	BodyPos = allPos;
	body->SetPosition(BodyPos);
	body->SetRotation(BodyRot);
	body->SetScale(BodyScl);
}

void Body::Update(bool arive,XMVECTOR allPos)
{
	if (arive == true) {
	}
	SetPRS(allPos);
	body->Update();
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
