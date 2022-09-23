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
	if (arive == true) {
		RArm->Draw();
	}
}

void RightArm::Finalize()
{
	RArm.reset();
	delete RArmModel;
}

void RightArm::Attack(float attackT)
{
	if (attackT >= 10) {
		if (attack == false) {
			RArmRot.x += 10.0f;
			if (RArmRot.x >= 160.0f) {
				attack = true;
			}
		}
	}
	if (attack == true) {
		RArmRot.x -= 10.0f;
		if (RArmRot.x <= 90.0f) {
			RArmRot.x = 90.0f;
			attack = false;
		}
	}
}
