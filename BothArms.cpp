#include "BothArms.h"

void BothArms::Initialize()
{
	ArmModel = ObjModel::CreateFromOBJ("Arm");
	RArm = Object3d::Create(ArmModel);
	LArm = Object3d::Create(ArmModel);
}

void BothArms::SetPRS(const XMVECTOR& bodyPos, Bullet* bull)
{
	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	Hit = bull->GetTrigger();
	//‰EŽè
	RArmPos = bodyPos;
	RArmPos.m128_f32[0] = bodyPos.m128_f32[0] + 0.8;
	RArmPos.m128_f32[2] = bodyPos.m128_f32[2] - 0.3f;
	//¶Žè
	LArmPos = bodyPos;
	LArmPos.m128_f32[0] = bodyPos.m128_f32[0] - 0.8f;
	LArmPos.m128_f32[2] = bodyPos.m128_f32[2] - 0.3f;

	RArm->SetPosition(RArmPos);
	RArm->SetRotation(ArmRot);
	RArm->SetScale(ArmScl);

	LArm->SetPosition(LArmPos);
	LArm->SetScale(ArmScl);
	LArm->SetRotation(ArmRot);
}

void BothArms::Update(bool& arive, const XMVECTOR& bodyPos, Bullet* bull, int& Hp)
{
	if (arive == true) {
		SetPRS(bodyPos, bull);
		if (Collision::ArmHit(LArmPos, ArmScl, bullPos, bullScl) || Collision::ArmHit(RArmPos, ArmScl, bullPos, bullScl)) {
			Hp -= 10;
			HitCount += 1;
			Hit = false;
			bull->SetTrigger(Hit);
		}
	}
	else if (arive == false) {
		ArmRot.x = 90.0f;
	}
	if (HitCount >= 3) {
		HitCount = 0;
		arive = false;
	}
	LArm->Update();
	RArm->Update();
}

void BothArms::Draw(bool arive)
{
	if (arive == true) {
		RArm->Draw();
		LArm->Draw();
	}
}

void BothArms::Finalize()
{
}

void BothArms::Attack(float& attackT,bool& AttackFase,int& playerHp)
{

	if (attackT >= 10 && attack == false) {
		ArmRot.x += 0.1f;
		//attackT = 0.0f;
		if (ArmRot.x >= 160.0f) {
			attack = true;
		}
	}
	if (attack == true) {
		ArmRot.x -= 10.0f;
		if (ArmRot.x <= 90.0f) {
			ArmRot.x = 90.0f;
			attack = false;
			AttackFase = false;
			attackT = 0;
			playerHp -= 1;
		}
	}
}
