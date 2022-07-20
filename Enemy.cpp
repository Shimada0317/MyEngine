#include "Enemy.h"
#include<cassert>
#include"imgui/imgui.h"

void Enemy::Initalize()
{
	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();
	model = ObjModel::CreateFromOBJ("block0");
	enemy->CreateGraphicsPipeline(L"Resources/shaders/BasicVS.hlsl", L"Resources/shaders/BasicPS.hlsl");
	enemy= Object3d::Create();
	enemy->SetModel(model);

	bull = new Bullet;
	bull->Initialize();

	bulpos = bull->GetPosition();
	bulscl = bull->GetScale();

}

void Enemy::Set()
{

	bull->SetPosition(bulpos);
	bull->SetScale(bulscl);
	enemy->SetPosition(position);

}

void Enemy::Update()
{
	Set();
	if (Collision::Player2Other(bulpos, bulscl, position, scale)) {
		arive = false;
	}

	
	enemy->Update();
}

void Enemy::Draw()
{
	Active();
}

void Enemy::ImGuiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Enemy");

	//フラグを手動で切りたい時
	ImGui::Checkbox("arive", &arive);
	//スライダーで動きをいじりたいとき

	if (ImGui::TreeNode("bull")) {
		ImGui::SliderFloat("position.x", &bulpos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("position.y", &bulpos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("position.z", &bulpos.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("position")) {
		ImGui::SliderFloat("position.x", &position.x, -100.0f, 100.0f);
		ImGui::SliderFloat("position.y", &position.y, -100.0f, 100.0f);
		ImGui::SliderFloat("position.z", &position.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Enemy::Finalize()
{
}

void Enemy::Active()
{
	if (arive == true) {
		enemy->Draw();
	}

}
