#include "LeftArm.h"

void LeftArm::Initialize()
{
	LArmModel = ObjModel::CreateFromOBJ("Arm");
	LArm = Object3d::Create();
	LArm->SetModel(LArmModel);
}

void LeftArm::SetPRS(XMVECTOR bodyPos)
{
	LArmPos = bodyPos;
	LArmPos.m128_f32[0] = bodyPos.m128_f32[0] - 0.8f;
	LArmPos.m128_f32[2] = bodyPos.m128_f32[2] - 0.3f;
	LArm->SetPosition(LArmPos);
	LArm->SetScale(LArmScl);
	LArm->SetRotation(LArmRot);
}

void LeftArm::Update(bool arive, XMVECTOR bodyPos)
{
	if (arive == true) {
		SetPRS(bodyPos);
	}
	LArm->Update();
}

void LeftArm::Draw(bool arive)
{

		LArm->Draw();
}

void LeftArm::Finalize()
{
	LArm.reset();
	delete LArmModel;
}

void LeftArm::Attack(int attackT)
{
	if (attackT <= 10) {
		LArmRot.y += 10;
	}
}
