#include "BothArms.h"

BothArms::~BothArms()
{
	delete ArmModel;
}

void BothArms::Initialize()
{
	ArmModel = ObjModel::CreateFromOBJ("BothArm");
	Arm = Object3d::Create(ArmModel);
}

void BothArms::SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull)
{
	//腕のステータス のセット
	Arm_Pos = bodyPos;
	Arm_Pos.m128_f32[1] = bodyPos.m128_f32[1]+0.2f;
	Arm_Pos.m128_f32[2] = bodyPos.m128_f32[2] ;
	Arm->SetPosition(Arm_Pos);
	Arm->SetRotation(Arm_Rot);
	Arm->SetScale(Arm_Scl);
}

void BothArms::Updata(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull, int& Hp)
{
	if (Arm_Rot.x >= 40) {
		Arm_Rot.x = 0;
	}

	if (arive == true) {
		Color.w = 1.0f;
		SetPRS(bodyPos,rotation, bull);
	}
	else if (arive == false) {
		Arm_Scl = { 0.2f,0.2f,0.2f };
		Arm_Rot.x = 90.0f;
		Color.x = 1.0f;
		Color.y = 1.0f;
		Color.z = 1.0f;
		Color.w -= 0.01f;
	}

	Arm->Updata(Color);
}

void BothArms::Draw(bool arive)
{
	if (Color.w >= 0) {
		Arm->Draw();
	}
}

void BothArms::RespownSet(const XMFLOAT3& rot)
{
	Arm_Rot = rot;
	Arm->SetRotation(Arm_Rot);
}

void BothArms::Attack(float& attackT,bool& AttackFase,int& playerHp, const bool& arive)
{
	if (arive == true) {
		if (attackT >= 10 && Attack_F == false) {
			Arm_Rot.x += 0.05f;
			//attackT = 0.0f;
			Arm_Scl.x += 0.00002f;
			Arm_Scl.y += 0.00002f;
			Arm_Scl.z += 0.00002f;
			Color.y -= 0.01f;
			Color.z -= 0.01f;
			if (Arm_Rot.x >= 40.0f) {
				Attack_F = true;
			}
		}
		if (Attack_F == true) {
			Arm_Rot.x -= 1.0f;
			if (Arm_Rot.x <= 0.0f) {
				Arm_Rot.x = 0.0f;
				Color = { 1.0f,1.0f,1.0f,1.0f };
				Arm_Scl = { 0.2f,0.2f,0.2f };
				Attack_F = false;
				AttackFase = false;
				attackT = 0;
				playerHp -= 1;
			}
		}
	}

	else {
		Attack_F = false;
		Arm_Rot.x = 0.0f;
	}
}
