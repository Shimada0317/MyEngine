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
		life[i] = enemy[i]->GetLife();
		enemy[i]->SetPlayer(player);
		stop[i] = false;
	}
}

void middle::SetPSR()
{
	player->SetPosition(playerPos);
	
	bull->SetPosition(bullPos);
	bull->SetScl(bullScl);
	bull->SetLost(lost);
	for (int i = 0; i < 2; i++) {
		if (life[i] <= 0) {
			enemyPos[i] = enemy[i]->GetPosition();
			enemy[i]->SetPosition(enemyPos[i]);
			enemy[i]->SetScl(enemyScl);
			life[i] = enemy[i]->GetLife();
		}
	}
}
	

void middle::AllUpdate()
{
	player->Update();
	for (int i = 0; i < 2; i++) {
		enemy[i]->Update(playerPos);
		enemyPos[i] = enemy[i]->GetPosition();
	}
	bull->Update();
}

void middle::Update()
{
		for (int i = 0; i < 2; i++) {
			if (life[i] >= 0) {
				if (Collision::Player2Other(bullPos, bullScl, enemyPos[i], enemyScl)) {
					life[i] -= 1;
					lost = true;
					shot = false;
					bullPos.z = -10;
					speed = 0;
					enemy[i]->SetLife(life[i]);
					stop[i] = true;
					if (life[i] <= 0) {
						count = true;
					}
				}
				else {
					lost = false;
					enemy[i]->GetSpeed();
				}
				if(Collision::HeadShot(bullPos, bullScl, enemyPos[i], enemyScl)) {
					life[i] -= 3;
					lost = true;
					shot = false;
					bullPos.z = -10;
					speed = 0;
					enemy[i]->SetLife(life[i]);
					stop[i] = true;
					if (life[i] <= 0) {
						count = true;
					}
				}
				else {
					lost = false;
					enemy[i]->GetSpeed();
				}
			
				if (playerPos.z >= enemyPos[i].z) {
					life[i] = 0;
					enemy[i]->SetLife(life[i]);
				}

				enemy[i]->Active(stop[i],1,playerPos);
			}
		}
		if (count == true) {
			hit += 1;
			count = false;
		}

		
		if (hit == 2) {
			patern += 1;
			cammove = 0.1f;
			hit = 0;
		}

		if (patern != 0) {
			playerPos.z += cammove;
			life[1] -= 3;
			life[0] -= 3;
			if (playerPos.z >= 10*patern) {
				cammove = 0;
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

	//if (Remaining >= 0) {
		bull->bun(bullPos, playerPos, speed, shot, Remaining);
	//}
		if (Input::GetInstance()->PushKey(DIK_R)) {
			Remaining = 9;
		}
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
	float r = Remaining;
	float p = patern;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("mouth");
	ImGui::Checkbox("shot", &shot);
	ImGui::Checkbox("stop", &stop[0]);
	ImGui::Checkbox("stop", &stop[1]);
	if (ImGui::TreeNode("playerPos")) {
		ImGui::SliderFloat("playerPos.z", &enemyPos[0].z, -100.0f, 100.0f);
		ImGui::SliderFloat("playerPos.z", &enemyPos[1].z, -100.0f, 100.0f);
		ImGui::SliderFloat("playerPos.z", &playerPos.z, -100.0f, 100.0f);
		ImGui::SliderFloat("Pos.y", &bullPos.z, -100.0f, 100.0f);
	
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("other")) {
		ImGui::SliderFloat("speed", &speed, -100.0f, 100.0f);
		ImGui::SliderFloat("Remaining", &r, -100.0f, 100.0f);
		ImGui::SliderFloat("hit", &hit, -100.0f, 100.0f);
		ImGui::SliderFloat("patern", &p, -100.0f, 100.0f);
		ImGui::Checkbox("count", &count);
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	enemy[0]->ImGuiDraw();
	enemy[1]->ImGuiDraw();
}

void middle::Fainalize()
{
}
