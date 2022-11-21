#include "BothArms.h"

BothArms::~BothArms()
{
	delete ArmModel;
}

void BothArms::Initialize()
{
	ArmModel = ObjModel::CreateFromOBJ("BothArm");
	RArm = Object3d::Create(ArmModel);
}

void BothArms::SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull)
{
	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	Hit = bull->GetTrigger();
	//‰EŽè
	RArmPos = bodyPos;
	RArmPos.m128_f32[1] = bodyPos.m128_f32[1]+0.2f;
	RArmPos.m128_f32[2] = bodyPos.m128_f32[2] ;
	
	

	RArm->SetPosition(RArmPos);
	RArm->SetRotation(ArmRot);
	RArm->SetScale(ArmScl);
}

void BothArms::Updata(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull, int& Hp)
{
	if (ArmRot.x >= 40) {
		ArmRot.x = 0;
	}

	if (arive == true) {
		Color.w = 1.0f;
		SetPRS(bodyPos,rotation, bull);
	}
	else if (arive == false) {
		ArmScl = { 0.2f,0.2f,0.2f };
		ArmRot.x = 90.0f;
		Color.x = 1.0f;
		Color.y = 1.0f;
		Color.z = 1.0f;
		Color.w -= 0.01f;
	}
	if (HitCount >= 3) {
		HitCount = 0;
		arive = false;
	}

	RArm->Updata(Color);
}

void BothArms::Draw(bool arive)
{
	if (Color.w >= 0) {
		RArm->Draw();
	}
}

void BothArms::Finalize()
{
}

void BothArms::RespownSet(const XMFLOAT3& rot)
{
	ArmRot = rot;
	RArm->SetRotation(ArmRot);
}

void BothArms::Attack(float& attackT,bool& AttackFase,int& playerHp, const bool& arive)
{
	if (arive == true) {
		if (attackT >= 10 && attack == false) {
			ArmRot.x += 0.05f;
			//attackT = 0.0f;
			ArmScl.x += 0.00002f;
			ArmScl.y += 0.00002f;
			ArmScl.z += 0.00002f;
			Color.y -= 0.01f;
			Color.z -= 0.01f;
			if (ArmRot.x >= 40.0f) {
				attack = true;
			}
		}
		if (attack == true) {
			ArmRot.x -= 1.0f;
			if (ArmRot.x <= 0.0f) {
				ArmRot.x = 0.0f;
				Color = { 1.0f,1.0f,1.0f,1.0f };
				ArmScl = { 0.2f,0.2f,0.2f };
				attack = false;
				AttackFase = false;
				attackT = 0;
				playerHp -= 1;
			}
		}
	}

	else {
		attack = false;
		ArmRot.x = 0.0f;
	}
}
