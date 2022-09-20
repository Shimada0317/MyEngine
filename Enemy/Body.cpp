#include "Body.h"

void Body::Initialize()
{
	bodyModel = ObjModel::CreateFromOBJ("enemy");
	body = Object3d::Create();
	body->SetModel(bodyModel);
	body.reset();
}

void Body::SetPRS()
{
	body->SetPosition(BodyPos);
	body->SetRotation(BodyRot);
	body->SetScale(BodyScl);
}

void Body::Update(bool arive)
{
	if (arive == true) {
		SetPRS();
	}
}

void Body::Draw(bool arive)
{
	if (arive == true) {
		body->Draw();
	}
}

void Body::Finalize()
{
	delete bodyModel;
}
