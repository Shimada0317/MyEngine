#include "middle.h"
#include"imgui/imgui.h"

void middle::Initialize()
{
	player = new Player();
	player->Initalize();

	playerPos = player->GetPosition();
	
	bull = new Bullet();
	bull->Initialize();


	bullPos = bull->GetPosition();
	bullScl = bull->GetScl();
	lost = bull->GetLost();

	for (int i = 0; i < 2; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initalize();
		enemyPos[i] = enemy[i]->GetPosition();
		enemyScl = enemy[i]->GetScl();
		life = enemy[i]->GetLife();
	}
}

void middle::SetPSR()
{
	player->SetPosition(playerPos);
	
	bull->SetPosition(bullPos);
	bull->SetScl(bullScl);
	bull->SetLost(lost);
	for (int i = 0; i < 2; i++) {
		if (life <= 0) {
			enemyPos[i] = enemy[i]->GetPosition();
			enemy[i]->SetPosition(enemyPos[i]);
			enemy[i]->SetScl(enemyScl);
			life = enemy[i]->GetLife();
		}
		
	}
}
	

void middle::AllUpdate()
{
	player->Update();
	for (int i = 0; i < 2; i++) {
		enemy[i]->Update();
	}
	bull->Update();
}

void middle::Update()
{

		for (int i = 0; i < 2; i++) {
			if (Collision::Player2Other(bullPos, bullScl, enemyPos[i], enemyScl)) {
				life -= 1;
				lost = true;
				shot = false;
				bullPos.z = -10;
				speed = 0;
				enemy[i]->SetLife(life);
			}
			else {
				lost = false;
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

	
	bull->bun(bullPos, playerPos, speed, shot);

	SetPSR();
	AllUpdate();
}

void middle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	bull->Draw();
	for (int i = 0; i < 2; i++) {
		enemy[i]->Draw();
	}
	//player->Draw(cmdList);
	player->ObjDraw();
}

void middle::ImGuiDraw()
{
	player->ImGuiDraw();
	float l = life;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("mouth");
	ImGui::Checkbox("shot", &shot);
	if (ImGui::TreeNode("playerPos")) {
		ImGui::SliderFloat("playerPos.x", &playerPos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("playerPos.y", &playerPos.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Pos.y", &bullPos.z, -100.0f, 100.0f);
		ImGui::SliderFloat("speed", &speed, -100.0f, 100.0f);
		ImGui::SliderFloat("life", &l, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	enemy[0]->ImGuiDraw();
}

void middle::Fainalize()
{
}
