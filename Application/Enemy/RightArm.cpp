#include "RightArm.h"

RightArm::~RightArm()
{
	delete RArmModel;
}

void RightArm::Initialize()
{
	RArmModel = ObjModel::CreateFromOBJ("Arm");
	RArm = Object3d::Create(RArmModel);
}

void RightArm::SetPRS(const XMVECTOR& bodyPos, Bullet* bull)
{
	RArmPos = bodyPos;
	RArmPos.m128_f32[0] = bodyPos.m128_f32[0] + 0.8;
	RArmPos.m128_f32[2] = bodyPos.m128_f32[2] - 0.3f;

	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	Hit = bull->GetTrigger();

	RArm->SetPosition(RArmPos);
	RArm->SetRotation(RArmRot);
	RArm->SetScale(RArmScl);
}

void RightArm::Updata(bool& arive, const XMVECTOR& bodyPos,Bullet* bull,int& Hp)
{
	if (arive == true) {
		SetPRS(bodyPos, bull);

		if (Collision::ArmHit(RArmPos, RArmScl, bullPos, bullScl)) {
			Hp -= 10;
			HitCount += 1;
			Hit = false;
			bull->SetTrigger(Hit);
		}
	}

	else if (arive == false) {
		RArmRot.x=0.0f;
	}

	if (HitCount >= 3) {
		HitCount = 0;
		arive = false;
	}

	RArm->Updata();
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
			RArmRot.x += 10.0f/20;
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
