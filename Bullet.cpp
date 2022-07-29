#include "Bullet.h"
#include"imgui/imgui.h"

void Bullet::Initialize()
{
	bulModel=ObjModel::CreateFromOBJ("white");
	bullet->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	bullet = Object3d::Create();
	bullet->SetModel(bulModel);
}

void Bullet::bun(XMFLOAT3& pos,XMFLOAT3 position, float& speed,bool& shot)
{
	XMFLOAT3 oldpos;
	oldpos.x = pos.x;
	oldpos.y = pos.y;
	oldpos.z = pos.z;
	if (retime == true) {
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			pos.x = position.x;
			pos.y = position.y;
			shot = true;
			retime = false;
			speed = 0.5f;
		}
	}
	if (shot == true) {
	
		pos.z += speed;
	}
	if (pos.z >= 30) {
		pos.z = -10;
		pos.x = oldpos.x;
		pos.y = oldpos.y;
		shot = false;
	}
	if (shot == false) {
		retime = true;
	}

}

void Bullet::Shot(XMFLOAT3& pos, XMFLOAT2 position, int speed, bool& shot)
{
		if (shot == true) {
			pos.x = position.x;
			pos.y = position.y;
			pos.z += speed;
			retime == false;
			if (pos.z >= 30) {
				pos.z = 0;
			}
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
	delete bullet;
	delete bulModel;
}

