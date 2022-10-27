#include "Bullet.h"
#include"imgui/imgui.h"

Bullet::~Bullet()
{
	delete bulModel;
}

void Bullet::Initialize()
{

	bullet.reset(new Object3d);
	bulModel = new ObjModel;

	bulModel = ObjModel::CreateFromOBJ("bullet");
	bullet = Object3d::Create(bulModel);
	mat = bullet->GetMatrix();
}

void Bullet::Stanby(const XMVECTOR& playerPos, const XMVECTOR& vel)
{
	velocity_ = vel;

	pos = playerPos;

}

void Bullet::bun(XMVECTOR& pos, XMVECTOR position, float& speed, bool& shot, int& Remaining, bool& retime)
{
	XMFLOAT3 oldpos;
	oldpos.x = pos.m128_f32[0];
	oldpos.y = pos.m128_f32[1];
	oldpos.z = pos.m128_f32[2];
	if (retime == true) {
		pos.m128_f32[0] = position.m128_f32[0];
		pos.m128_f32[1] = position.m128_f32[1];
		pos.m128_f32[2] = position.m128_f32[2];
		shot = true;
		retime = false;
		speed = 1.0f;
		Remaining -= 1;
	}
	if (Remaining >= 0) {
		if (shot == true) {
			pos.m128_f32[2] += speed;
		}
		if (pos.m128_f32[2] >= 30 + position.m128_f32[2]) {
			pos.m128_f32[2] = -10;
			pos.m128_f32[0] = oldpos.x;
			pos.m128_f32[2] = oldpos.y;
			shot = false;
		}
		if (shot == false) {
			retime = true;
		}
	}
	else {
		retime = true;
		pos.m128_f32[2] = -10;
		shot = false;
		Remaining = 0;
	}
}

void Bullet::Shot(XMVECTOR& pos, XMFLOAT2 position, int speed, bool& shot)
{
	if (shot == true) {
		pos.m128_f32[0] = position.x;
		pos.m128_f32[1] = position.y;
		pos.m128_f32[2] += speed;

		if (pos.m128_f32[2] >= 30) {
			pos.m128_f32[2] = 0;
		}
	}


}

void Bullet::debug(XMFLOAT3& pos, XMFLOAT3 posiiton, int speed, bool& d)
{
	XMFLOAT3 oldpos;
	if (d == false) {
		oldpos.x = pos.x;
		oldpos.y = pos.y;
		oldpos.z = pos.z;
	}
	if (d == true) {
		pos.z += 0.1f;
	}

	if (pos.z >= 30 + posiiton.z) {
		pos.z = -10;
		pos.x = oldpos.x;
		pos.y = oldpos.y;
		d = false;
	}
}

void Bullet::Test(const XMVECTOR& GWorldPos, const XMVECTOR& PWorldPos)
{

	const float kBullSpeed = 1.0f;
	if (Trigger == false) {
		ShotT = 0;
		pos.m128_f32[0] = GWorldPos.m128_f32[0];
		pos.m128_f32[1] = GWorldPos.m128_f32[1];
		pos.m128_f32[2] = GWorldPos.m128_f32[2];
		fire = true;
	}
	XMVECTOR vec = PWorldPos -GWorldPos;
	vec = XMVector3Normalize(vec);
	vec *= kBullSpeed;
	velocity_.m128_f32[0] = vec.m128_f32[0] / 3;
	velocity_.m128_f32[1] = vec.m128_f32[1] / 3;
	velocity_.m128_f32[2] = vec.m128_f32[2] / 2;
}

void Bullet::Set()
{
	bullet->SetPosition({ pos });
	bullet->SetRotation(rot);
	bullet->SetScale(scl);
}

void Bullet::ShotBefore(const XMVECTOR& startPos, const XMVECTOR& velocity)
{

	if (Trigger == false) {
		pos.m128_f32[0] = startPos.m128_f32[0];
		pos.m128_f32[1] = startPos.m128_f32[1];
		pos.m128_f32[2] = startPos.m128_f32[2];
		fire = true;
	}
	velocity_ = velocity;
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

void Bullet::ShotAfter(const XMVECTOR& baclplayerpos, const XMVECTOR& playerPos, const XMVECTOR& PlayerWorldPos,int& remaining)
{
	float speedm = 1.5;


	if (Trigger == true) {
		if (fire == true) {
			//’Ç”ö‚ÌŽ®‚Ì“r’†
			float vx = (baclplayerpos.m128_f32[0] - PlayerWorldPos.m128_f32[0]);
			float vy = (baclplayerpos.m128_f32[1] - PlayerWorldPos.m128_f32[1]);
			float vz = (baclplayerpos.m128_f32[2] - PlayerWorldPos.m128_f32[2]);
			float v2x = pow(vx, 2);
			float v2y = pow(vy, 2);
			float v2z = pow(vz, 2);
			float l = sqrtf(v2x + v2y + v2z);
			v3x = (vx / l) * velocity_.m128_f32[2];
			v3y = (vy / l) * velocity_.m128_f32[2];
			v3z = (vz / l) * velocity_.m128_f32[2];
			//remaining += 1;
			fire = false;
		}
	}

	pos.m128_f32[0] -= v3x;
	pos.m128_f32[1] -= (v3y / 3);
	pos.m128_f32[2] -= v3z;

	//’Ç”ö‘O‚Ìó‘Ô
	/*verosity_ = { 0, 0, bullSpeed, 1 };
	bullPos[i].m128_f32[2] += verosity_.m128_f32[2];*/
	if (pos.m128_f32[2] >= 30 + PlayerWorldPos.m128_f32[2]) {
		pos.m128_f32[2] = -10;
		Trigger = false;
	}
}

void Bullet::Hit()
{

}


void Bullet::Update()
{

	pos += velocity_;
//	pos += velocity_;
	ShotT += 1.0f;
	if (ShotT >= 60) {
		Trigger = false;
		ShotT = 0;
	}



	Set();
	bullet->Update();
}

void Bullet::Draw()
{
	if (Trigger == true) {
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

