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


}

void Enemy::Set()
{

	enemy->SetPosition(position);
	enemy->SetRotation(rotation);
	enemy->SetScale(scale);

}

void Enemy::Update()
{
	if (life <= 0) {
		arive = false;
	}

	if (arive == false) {
		position.x = rand() % 10 - 5;
		position.z = rand() % 1 + 20;
	}
	

	Set();
	enemy->Update();
}

void Enemy::Draw()
{
	if (arive == true) {
		enemy->Draw();
	}
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

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void Enemy::Finalize()
{
}

