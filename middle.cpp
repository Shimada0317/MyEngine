#include "middle.h"
#include"imgui/imgui.h"

void middle::Initialize()
{
	player = new Player();
	player->Initalize();

	enemy = new Enemy();
	enemy->Initalize();

	bull = new Bullet();
	bull->Initialize();

	playerPos = player->GetPosition();

	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();

	enemyPos = enemy->GetPosition();
	enemyScl = enemy->GetScl();
	arive = enemy->GetArive();

}

void middle::SetPSR()
{
	player->SetPosition(playerPos);
	
	bull->SetPosition(bullPos);
	bull->SetScl(bullScl);
	if (arive == false) {
		enemyPos = enemy->GetPosition();
	}
	enemy->SetPosition(enemyPos);
	enemy->SetScl(enemyScl);
	enemy->SetArive(arive);
	
}
	

void middle::AllUpdate()
{
	player->Update();
	enemy->Update();
	bull->Update();
}

void middle::Update()
{

	if (Collision::Player2Other(bullPos, bullScl, enemyPos, enemyScl)) {
		arive = false;
	}

	if (arive == false) {
		responetime += 0.2f;
		if (responetime >= 10.0f) {
			responetime = 0.0f;
			arive = true;
		}
	}
	Action::GetInstance()->PlayerMove3d(playerPos, 0.2f);
	

	if (playerPos.y <= 0.0f) {
		playerPos.y = 0.0f;
	}
	else if (playerPos.y >= 4.2f) {
		playerPos.y = 4.2f;
	}

	if (playerPos.x >= 6.8f) {
		playerPos.x = 6.8f;
	}
	else if (playerPos.x <= -6.8f) {
		playerPos.x = -6.8f;
	}

	
	bull->bun(bullPos, playerPos, 0.5f, shot);

	SetPSR();
	AllUpdate();
}

void middle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	bull->Draw();
	enemy->Draw();
	//player->Draw(cmdList);
	player->ObjDraw();
}

void middle::ImGuiDraw()
{
	player->ImGuiDraw();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("mouth");

	if (ImGui::TreeNode("playerPos")) {
		ImGui::SliderFloat("playerPos.x", &playerPos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("playerPos.y", &playerPos.y, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	enemy->ImGuiDraw();
}

void middle::Fainalize()
{
}
