#include "RightArm.h"

void RightArm::Initialize()
{
	RArmModel = ObjModel::CreateFromOBJ("Arm");
	RArm = Object3d::Create();
	RArm->SetModel(RArmModel);
}

void RightArm::SetPRS(XMVECTOR bodyPos)
{
	RArmPos = bodyPos;
	RArmPos.m128_f32[0] = bodyPos.m128_f32[0] + 0.8;
	RArmPos.m128_f32[2] = bodyPos.m128_f32[2] - 0.3f;
	RArm->SetPosition(RArmPos);
	RArm->SetRotation(RArmRot);
	RArm->SetScale(RArmScl);
}

void RightArm::Update(bool arive, XMVECTOR bodyPos)
{
	if (arive == true) {
		SetPRS(bodyPos);
	}
	RArm->Update();
}

void RightArm::Draw(bool arive)
{

		RArm->Draw();

}

void RightArm::Finalize()
{
	RArm.reset();
	delete RArmModel;
}

void RightArm::Attack(int attackT)
{
	if (attackT <= 10) {
		RArmRot.y += 10;
	}
}
