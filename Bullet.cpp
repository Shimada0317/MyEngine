#include "Bullet.h"
#include"imgui/imgui.h"

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

void Bullet::Test(const XMVECTOR& position, const XMVECTOR& velocity)
{
	pos = position;
	velocity_ = velocity;
}

void Bullet::Set()
{
	bullet->SetPosition({ pos });
	bullet->SetRotation(rot);
	bullet->SetScale(scl);
	mat = bullet->GetMatrix();
}

void Bullet::ShotBefore(const XMVECTOR& startPos)
{

	if (Trigger == false) {
		pos.m128_f32[0] = startPos.m128_f32[0];
		pos.m128_f32[1] = startPos.m128_f32[1];
		pos.m128_f32[2] = startPos.m128_f32[2];
		fire = true;
	}

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

void Bullet::ShotAfter(const XMVECTOR& baclplayerpos, const XMVECTOR& playerPos,int& remaining)
{
	float speedm = 5.1;


	if (Trigger == true) {
		if (fire == true) {
			//’Ç”ö‚ÌŽ®‚Ì“r’†
			float vx = (baclplayerpos.m128_f32[0] - playerPos.m128_f32[0]);
			float vy = (baclplayerpos.m128_f32[1] - playerPos.m128_f32[1]);
			float vz = (baclplayerpos.m128_f32[2] - playerPos.m128_f32[2]);
			float v2x = pow(vx, 2);
			float v2y = pow(vy, 2);
			float v2z = pow(vz, 2);
			float l = sqrtf(v2x + v2y + v2z);
			v3x = (vx / l) * speedm;
			v3y = (vy / l) * speedm;
			v3z = (vz / l) * speedm;
			//remaining += 1;
			fire = false;
		}
	}

	pos.m128_f32[0] -= v3x;
	pos.m128_f32[1] -= v3y;
	pos.m128_f32[2] -= v3z;

	//’Ç”ö‘O‚Ìó‘Ô
	/*verosity_ = { 0, 0, bullSpeed, 1 };
	bullPos[i].m128_f32[2] += verosity_.m128_f32[2];*/
	if (pos.m128_f32[2] >= 30 + playerPos.m128_f32[2]) {
		pos.m128_f32[2] = -10;
		Trigger = false;
	}
}

void Bullet::Hit()
{

}


void Bullet::Update()
{
	//pos.m128_f32[0] += velocity_.m128_f32[0];
	//pos.m128_f32[1] += velocity_.m128_f32[1];
	/*pos += velocity_;

	rot.x++;*/
	pos += velocity_;
	ShotT += 0.1f;
	if (ShotT >= 30) {
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

