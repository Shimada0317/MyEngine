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
	playerScl = player->GetScl();
	playerRot = player->GetRotation();

	enemyPos = enemy->GetPosition();
	enemyScl = enemy->GetScl();
	enemyRot = enemy->GetRotation();
	
	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	bullRot = bull->GetRotation();
}

void middle::SetPSR()
{
	player->SetPosition(playerPos);
	player->SetScl(playerScl);
	player->SetRotation(playerRot);

	enemy->SetPosition(enemyPos);
	enemy->SetScl(enemyScl);
	enemy->SetRotation(enemyRot);

	bull->SetPosition(bullPos);
	bull->SetScl(bullScl);
	bull->SetRotation(bullRot);
	
	Input::GetInstance()->SetPosition(playerPos.x, playerPos.y);
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

	Action::GetInstance()->PlayerMove2d(playerPos, moveSpeed);
	Action::GetInstance()->Gunshot(2, shot);
	bull->bun(bullPos, playerPos,1, shot);

	//Action::GetInstance()->PlayerMove2d(rotation, 0.1f);
	if (playerPos.y <= 0.5f) {
		playerPos.y = 0.5f;
	}
	else if (playerPos.y >= 6.2f) {
		playerPos.y = 6.2f;
	}

	if (playerPos.x >= 9.8f) {
		playerPos.x = 9.8f;
	}
	else if (playerPos.x <= -9.8f) {
		playerPos.x = -9.8f;
	}

	

	SetPSR();
	AllUpdate();
}

void middle::Draw()
{
	bull->Draw();
	if (arive == true) {
		enemy->Draw();
	}
	player->ObjDraw();
}

void middle::ImGuiDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Player");
	bool blnCk = false;
	//フラグを手動で切りたい時

	//スライダーで動きをいじりたいとき
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Enemy");
	//フラグを手動で切りたい時
	ImGui::Checkbox("arive", &arive);
	//スライダーで動きをいじりたいとき
	if (ImGui::TreeNode("position")) {
		ImGui::SliderFloat("position.x", &enemyPos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("position.y", &enemyPos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("position.z", &enemyPos.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("bull");
	//フラグを手動で切りたい時
	ImGui::Checkbox("shot", &shot);
	//スライダーで動きをいじりたいとき

	if (ImGui::TreeNode("bull")) {
		ImGui::SliderFloat("position.x", &bullPos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("position.y", &bullPos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("position.z", &bullPos.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	if (ImGui::TreeNode("position")) {
		ImGui::SliderFloat("player.x", &playerPos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("player.y", &playerPos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("player.z", &playerPos.z, -100.0f, 100.0f);

		ImGui::TreePop();
	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void middle::Fainalize()
{
}
