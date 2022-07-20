#include "Bullet.h"
#include"imgui/imgui.h"

void Bullet::Initialize()
{
	bulModel=ObjModel::CreateFromOBJ("white");
	bullet->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	bullet = Object3d::Create();
	bullet->SetModel(bulModel);
}

void Bullet::bun(XMFLOAT3& pos,XMFLOAT3 position, int speed,bool& shot)
{
	//float b2pX, b2pY,b2p;
	if (shot == true) {
		/*b2pX = pos.x - position.x;
		b2pY = pos.y - position.y;
		b2p = sqrtf((b2pX * b2pX) + (b2pY * b2pY));
		pos.x = b2pX / b2p * 5;
		pos.y = b2pY / b2p * 10;*/
		pos.x = position.x;
		pos.y = position.y;
		pos.z+=speed;
		if (pos.z >= 30.0f) {
			shot = false;
			pos = oldpos;
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
	bullet->Draw();
}

void Bullet::ImgUiDraw()
{

}

void Bullet::Finalize()
{
	delete bullet;
	delete bulModel;
}

