#include "Head.h"

Head::~Head()
{
	delete headModel;
}

void Head::Initialize(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation)
{
	headModel = ObjModel::CreateFromOBJ("Head");
	Head = Object3d::Create(headModel);

	
	HeadPos = bodyPos;
	HeadRot = rotation;
	HeadPos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(HeadPos);
	Head->SetRotation(HeadRot);
	Head->SetScale(HeadScl);

	MotionRad = (rand() % 2);
}

void Head::SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull)
{

	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	Hit = bull->GetTrigger();
	

	HeadPos = bodyPos;

	HeadPos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(HeadPos);

	Head->SetScale(HeadScl);
}

void Head::Updata(bool& arive,const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull,int& Hp)
{
	if (arive == true) {
		//SetPRS(bodyPos);
		SetPRS(bodyPos,rotation, bull);

		if (Collision::HeadHit(HeadPos, HeadScl, bullPos, bullScl)) {
			Hp -= 50;
			XMVECTOR pos = { 0.0f,1000.0f,0.0f };
			Hit = false;
			bull->SetTrigger(Hit);
			bull->SetPosition(pos);
			HitColor = { 1,0,0,1 };
		}
		else {
			HitColor = { 1,1,1,1 };
		}
	}
	else {
		HitColor.w -= 0.01f;
	}
	

	Head->Updata(HitColor);
}

void Head::Draw(bool arive)
{
	if (HitColor.w >= 0) {
		Head->Draw();
	}
}

void Head::Motion(const float& rot)
{

	if (MotionRad == 0) {
		if (radChange == true) {
			HeadRot.x += rot;
			//HeadRot.x += rot;
		}
		else {
			HeadRot.x -= rot;
			//HeadRot.z -= rot;
		}
		if (HeadRot.x >= 10) {
			radChange = false;
		}
		else if (HeadRot.x <= -10) {
			radChange = true;
		}
	}

	if (MotionRad == 1) {
		if (radChange == true) {
			//HeadRot.x += rot;
			HeadRot.z += rot;
		}
		else {
			//HeadRot.x -= rot;
			HeadRot.z -= rot;
		}
		if (HeadRot.z >= 10) {
			radChange = false;
		}
		else if (HeadRot.z <= -10) {
			radChange = true;
		}
	}


	Head->SetRotation(HeadRot);
}

void Head::Finalize()
{
	Head.reset();
	delete headModel;
}
