#include "Bullet.h"
#include"imgui/imgui.h"

void Bullet::Initialize()
{

	bullet.reset(new Object3d);
	bulModel = new ObjModel;

	bulModel = ObjModel::CreateFromOBJ("bullet");
	bullet=Object3d::Create();
	bullet->SetModel(bulModel);
}

void Bullet::bun(XMVECTOR& pos, XMVECTOR position, float& speed, bool& shot, int& Remaining,bool& retime)
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

void Bullet::Set()
{
	bullet->SetPosition(pos);
	bullet->SetRotation(rot);
	bullet->SetScale(scl);
}


void Bullet::Update()
{
	rot.x++;
	Set();
	bullet->Update();
}

void Bullet::Draw()
{
	if (lost == false) {
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

