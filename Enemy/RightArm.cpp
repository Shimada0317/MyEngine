#include "RightArm.h"

void RightArm::Initialize()
{
	RArmModel = ObjModel::CreateFromOBJ("enemy");
	RArm = Object3d::Create();
	RArm->SetModel(RArmModel);
	RArm.reset();
}

void RightArm::SetPRS(XMVECTOR bodyPos)
{
	RArmPos = bodyPos;
	RArmPos.m128_f32[0] = bodyPos.m128_f32[0] + 1;
	RArm->SetPosition(RArmPos);
	RArm->SetRotation(RArmRot);
	RArm->SetScale(RArmScl);
}

void RightArm::Update(bool arive, XMVECTOR bodyPos)
{
	if (arive == true) {
		SetPRS(bodyPos);
	}
}

void RightArm::Draw(bool arive)
{
	if (arive == true) {
		RArm->Draw();
	}
}

void RightArm::Finalize()
{
	delete RArmModel;
}
