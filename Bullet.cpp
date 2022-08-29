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

void Bullet::bun(XMFLOAT3& pos, XMFLOAT3 position, float& speed, bool& shot, int& Remaining,bool& retime)
{
	XMFLOAT3 oldpos;
	oldpos.x = pos.x;
	oldpos.y = pos.y;
	oldpos.z = pos.z;
	if (retime == true) {
		pos.x = position.x;
		pos.y = position.y;
		pos.z = position.z;
		shot = true;
		retime = false;
		speed = 1.0f;
		Remaining -= 1;
	}
	if (Remaining >= 0) {
		if (shot == true) {
			pos.z += speed;
		}
		if (pos.z >= 30 + position.z) {
			pos.z = -10;
			pos.x = oldpos.x;
			pos.y = oldpos.y;
			shot = false;
		}
		if (shot == false) {
			retime = true;
		}
	}
	else {
		retime = true;
		pos.z = -10;
		shot = false;
		Remaining = 0;
	}
}

void Bullet::Shot(XMFLOAT3& pos, XMFLOAT2 position, int speed, bool& shot)
{
	if (shot == true) {
		pos.x = position.x;
		pos.y = position.y;
		pos.z += speed;

		if (pos.z >= 30) {
			pos.z = 0;
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

