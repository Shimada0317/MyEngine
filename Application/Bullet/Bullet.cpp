#include "Bullet.h"
#include"imgui/imgui.h"

Bullet::~Bullet()
{
	delete bulModel;
	bullet.reset();
}

void Bullet::Initialize()
{

	bullet.reset(new Object3d);
	bulModel = new ObjModel;

	bulModel = ObjModel::CreateFromOBJ("bullet");
	bullet = Object3d::Create(bulModel);
	mat = bullet->GetMatrix();
}



void Bullet::Test(const XMVECTOR& GWorldPos, const XMVECTOR& PWorldPos,const XMFLOAT3& Eye_Rot, const XMFLOAT3& rotation)
{
	time = 0.0f;
	drawF = false;
	const float kBullSpeed = 0.7f;
	if (Trigger == false) {
		ShotT = 0;
		pos.m128_f32[0] = GWorldPos.m128_f32[0];
		pos.m128_f32[1] = GWorldPos.m128_f32[1];
		pos.m128_f32[2] = GWorldPos.m128_f32[2];

		oldpos.m128_f32[0] = PWorldPos.m128_f32[0];
		oldpos.m128_f32[1] = PWorldPos.m128_f32[1];
		oldpos.m128_f32[2] = PWorldPos.m128_f32[2]+2;
		fire = true;

		rot.x = Eye_Rot.x+rotation.x;
		rot.y = Eye_Rot.y + rotation.y;
		rot.z = Eye_Rot.z + rotation.z;

		if (count == 2 || count == 3) {
			rot.y = Eye_Rot.y + rotation.y+90;
		}
	}
	XMVECTOR vec = PWorldPos -GWorldPos;
	vec = XMVector3Normalize(vec);
	vec *= kBullSpeed;
	velocity_.m128_f32[0] = vec.m128_f32[0];
	velocity_.m128_f32[1] = vec.m128_f32[1];
	velocity_.m128_f32[2] = vec.m128_f32[2];
}

void Bullet::Set()
{
	bullet->SetPosition({ pos });
	bullet->SetRotation(rot);
	bullet->SetScale(scl);
}

bool Bullet::CheckOk()
{
	if (Trigger == false) {
		return true;
	}

	return false;
}

void Bullet::TriggerOn()
{
	Trigger = true;
}

void Bullet::Updata()
{
	time += 0.2f;
	if (time >= 1) {
		time = 1;
		drawF = true;
	}
	pos += velocity_;
	ShotT += 1.0f;
	if (ShotT >= 60) {
		Trigger = false;
	}

	Set();
	bullet->Updata();
}

void Bullet::Draw()
{
	if (Trigger == true&&drawF==true) {
		bullet->Draw();
	}

}

void Bullet::ImgUiDraw()
{

}

void Bullet::Finalize()
{

	delete bulModel;
}

