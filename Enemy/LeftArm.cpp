#include "LeftArm.h"

void LeftArm::Initialize()
{
	LArmModel = ObjModel::CreateFromOBJ("enemy");
	LArm = Object3d::Create();
	LArm->SetModel(LArmModel);
	LArm.reset();
}

void LeftArm::SetPRS(XMVECTOR bodyPos)
{
	LArmPos = bodyPos;
	LArmPos.m128_f32[0] = bodyPos.m128_f32[0] - 1;
	LArm->SetPosition(LArmPos);
	LArm->SetScale(LArmScl);
	LArm->SetRotation(LArmRot);
}

void LeftArm::Update(bool arive, XMVECTOR bodyPos)
{
	if (arive == true) {
		SetPRS(bodyPos);
	}
}

void LeftArm::Draw(bool arive)
{
	if (arive == true) {
		LArm->Draw();
	}
}

void LeftArm::Finalize()
{
	delete LArmModel;
}
