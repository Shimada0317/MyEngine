#include "Bullet.h"
#include"imgui/imgui.h"

void Bullet::Initialize()
{
	bulModel=ObjModel::CreateFromOBJ("white");
	bullet->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	bullet = Object3d::Create();
	bullet->SetModel(bulModel);
	bullet->SetPosition(pos);
	bullet->SetScale(scl);

	oldpos = bullet->GetPosition();
	
}

void Bullet::bun(XMFLOAT3 position)
{
	//float b2pX, b2pY,b2p;
	speed = 1.0f;
	Action::GetInstance()->Gunshot(2, shot);
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

	//Collision::Player2Other(pos,scl, )


}

void Bullet::SetPos()
{
	bullet->SetPosition(pos);
}


void Bullet::Update()
{
	SetPos();
	bullet->Update();
}

void Bullet::Draw()
{
	bullet->Draw();
}

void Bullet::ImgUiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Enemy");

	//スライダーで動きをいじりたいとき

	if (ImGui::TreeNode("position")) {
		ImGui::SliderFloat("speed", &speed, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

}

void Bullet::Finalize()
{
	delete bullet;
	delete bulModel;
}

