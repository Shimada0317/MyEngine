#include "LeftArm.h"

void LeftArm::Initialize()
{
	LArmModel = ObjModel::CreateFromOBJ("Arm");
	LArm = Object3d::Create();
	LArm->SetModel(LArmModel);
}

void LeftArm::SetPRS(XMVECTOR bodyPos,Bullet* bull)
{
	LArmPos = bodyPos;
	LArmPos.m128_f32[0] = bodyPos.m128_f32[0] - 0.8f;
	LArmPos.m128_f32[2] = bodyPos.m128_f32[2] - 0.3f;
	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();

	LArm->SetPosition(LArmPos);
	LArm->SetScale(LArmScl);
	LArm->SetRotation(LArmRot);
}

void LeftArm::Update(bool& arive, XMVECTOR bodyPos,Bullet* bull,int& Hp)
{
	if (arive == true) {
		SetPRS(bodyPos,bull);
		if (Collision::ArmHit(LArmPos, LArmScl, bullPos, bullScl)) {
			
			Hp -= 10;
			HitCount += 1;

		}
	}

	if (HitCount >= 3) {
		HitCount = 0;
		arive = false;
	}

	LArm->Update();
}

void LeftArm::Draw(bool& arive)
{
	if (arive == true) {
		LArm->Draw();
	}
}

void LeftArm::Finalize()
{
	LArm.reset();
	delete LArmModel;
}

void LeftArm::Attack(float attackT)
{
	if (attackT >= 10) {
		if (attack == false) {
			LArmRot.x += 10.0f;
			if (LArmRot.x >= 160.0f) {
				attack = true;
			}
		}
	}
	if (attack == true) {
		LArmRot.x -= 10.0f;
		if (LArmRot.x <= 90.0f) {
			LArmRot.x = 90.0f;
			attack = false;
		}
	}
	
}
