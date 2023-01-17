#include "Head.h"

Head::~Head()
{
	delete headModel;
}

void Head::Initialize(bool& arive, const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Camera* camera)
{
	headModel = ObjModel::CreateFromOBJ("Head");
	Head = Object3d::Create(headModel);

	//頭の初期ステータス
	Head_Pos = bodyPos;
	Head_Rot = rotation;
	Head_Pos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(Head_Pos);
	Head->SetRotation(Head_Rot);
	Head->SetScale(Head_Scl);

	MotionRand = (rand() % 2);
}

void Head::SetPRS(const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull)
{
	//弾のステータスGet
	Bull_Pos = bull->GetPosition();
	Bull_Scl = bull->GetScl();
	Hit_F = bull->GetTrigger();
	//頭のステータスSet
	Head_Pos = bodyPos;
	Head_Pos.m128_f32[1] = bodyPos.m128_f32[1] +1.0f;
	Head->SetPosition(Head_Pos);
	Head->SetScale(Head_Scl);
}

void Head::Updata(bool& arive,const XMVECTOR& bodyPos, const XMFLOAT3 rotation, Bullet* bull,int& Hp)
{
	if (arive == true) {
		//SetPRS(bodyPos);
		
		SetPRS(bodyPos, rotation, bull);
		if (Collision::HeadHit(Head_Pos, Head_Scl, Bull_Pos, Bull_Scl)) {
			Hp -= 50;
			Hit_F = false;
			bull->SetTrigger(Hit_F);
			HitColor = { 1,0,0,1 };
		}
		else {
			HitColor = { 1,1,1,1 };
		}
		SetPRS(bodyPos, rotation, bull);
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
	//首の回転縦
	if (MotionRand == 0) {
		if (RotationHead_F == true) {
			Head_Rot.x += rot;
			//Head_Rot.x += rot;
		}
		else {
			Head_Rot.x -= rot;
		}
		if (Head_Rot.x >= 10) {
			RotationHead_F = false;
		}
		else if (Head_Rot.x <= -10) {
			RotationHead_F = true;
		}
	}
	//首の回転横
	if (MotionRand == 1) {
		if (RotationHead_F == true) {
			Head_Rot.y += rot;
		}
		else {
			Head_Rot.y -= rot;
		}
		if (Head_Rot.y >= 10) {
			RotationHead_F = false;
		}
		else if (Head_Rot.y <= -10) {
			RotationHead_F = true;
		}
	}


	Head->SetRotation(Head_Rot);
}

void Head::Finalize()
{
	Head.reset();
	delete headModel;
}
