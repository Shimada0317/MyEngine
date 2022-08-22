#include "middle.h"
#include"imgui/imgui.h"

void middle::Initialize()
{
	//プレイヤー(レティクル)の読み込み
	player = new Player();
	player->Initalize();
	playerPos = player->GetPosition();

	//弾の読み込み
	for (int j = 0; j < 9; j++) {
		bull[j] = new Bullet();
		bull[j]->Initialize();

		retime[j] = true;
		bullPos[j] = bull[j]->GetPosition();
		bullScl = bull[j]->GetScl();
		lost = bull[j]->GetLost();
		debug[j] = 0;
		speed[j] = 0.5f;
	}
	//敵の読み込み
	for (int i = 0; i < 2; i++) {
		enemy[i] = new Enemy();
		enemy[i]->Initalize();
		enemyPos[i] = enemy[i]->GetPosition();
		enemyScl = enemy[i]->GetScl();
		life[i] = enemy[i]->GetLife();
		enemy[i]->SetPlayer(player);
		stop[i] = false;
	}

	////スプライトの読み込み
	for (int i = 0; i < 9; i++) {
		Sprite::LoadTexture(i, L"Resources/bullet.png");
		bulletHUD[i] = Sprite::SpriteCreate(i, {10.0f,10.0f});
	}

	Sprite::LoadTexture(10, L"Resources/reload.png");
	Reload = Sprite::SpriteCreate(10, { 10.0f,10.0f }, {1.0f,1.0f,1.0f,1.0f});

	Sprite::LoadTexture(11, L"Resources/wave.png");
	wave = Sprite::SpriteCreate(11, { 10.0f,10.0f });

	Sprite::LoadTexture(17, L"Resources/five.png");
	maxcount = Sprite::SpriteCreate(17, { 10.0f,10.0f });

	Sprite::LoadTexture(12, L"Resources/slash.png");
	slash = Sprite::SpriteCreate(12, { 10.0f,10.0f });

	Sprite::LoadTexture(13, L"Resources/one.png");
	Sprite::LoadTexture(14, L"Resources/two.png");
	Sprite::LoadTexture(15, L"Resources/three.png");
	Sprite::LoadTexture(16, L"Resources/four.png");
	for (int i = 0; i < 5; i++) {
		changecount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}

}

void middle::SetPSR()
{
	//HUDのポジションセット
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ spSiz });
		bulletHUD[i]->SetPosition({ spPos.x,spPos.y + 32 * i });
	}

	Reload->SetSize({ 128,64 });
	Reload->SetPosition({ 1140,300 });

	wave->SetSize({ 256,128 });
	wave->SetPosition({ 0,600 });

	maxcount->SetSize({ 80,80 });
	maxcount->SetPosition({ 320, 630 });

	slash->SetSize({ 80,80 });
	slash->SetPosition({ 280,630 });

	for (int i = 0; i < 5; i++) {
		changecount[i]->SetSize({ 80,80 });
		changecount[i]->SetPosition({ 240,630 });
	}

	//プレイヤーのポジションセット
	player->SetPosition(playerPos);
	for (int j = 0; j < 9; j++) {
		bull[j]->SetPosition(bullPos[j]);
		bull[j]->SetScl(bullScl);
		bull[j]->SetLost(lost);
	}
	//敵のポジションセット
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
	//プレイヤーの更新
	player->Update();

	//敵の更新
	for (int i = 0; i < 2; i++) {
		enemy[i]->Update(playerPos);
		enemyPos[i] = enemy[i]->GetPosition();
	}
	//弾の更新
	for (int j = 0; j < 9; j++) {
		bull[j]->Update();
	}
}

void middle::Update()
{
	for (int i = 0; i < 2; i++) {
		if (life[i] >= 0) {
			for (int j = 0; j < 9; j++) {
				if (Collision::Player2Other(bullPos[j], bullScl, enemyPos[i], enemyScl)) {
					life[i] -= 1;
					lost = true;
					shot[j] = false;
					bullPos[j].z = -10;
					speed[j] = 0;
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
				if (Collision::HeadShot(bullPos[j], bullScl, enemyPos[i], enemyScl)) {
					life[i] -= 3;
					lost = true;
					shot[j] = false;
					bullPos[j].z = -10;
					speed[j] = 0;
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
			}
			if (playerPos.z >= enemyPos[i].z) {
				life[i] = 0;
				enemy[i]->SetLife(life[i]);
			}

			enemy[i]->Active(stop[i], 1, playerPos);

		}
	}
	//敵を倒した時、hitカウントを上げる
	if (count == true) {
		hit += 1;
		count = false;
	}

	//hitカウントが2になった時、ウェーブを進める
	if (hit == 2) {
		patern += 1;
		cammove = 0.1f;
		hit = 0;
	}

	if (patern != 0) {
		playerPos.z += cammove;
		life[1] -= 3;
		life[0] -= 3;
		if (playerPos.z >= 10 * patern) {
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


	if (Remaining < 8 && ReloadFlag == false) {
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			Remaining += 1;
			for (int i = 0; i < 9; i++) {
				if (shot[i] == false) {
					bullPos[i].x = playerPos.x;
					bullPos[i].y = playerPos.y;
					bullPos[i].z = playerPos.z;
					shot[i] = true;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		if (shot[i] == true) {
			bullPos[i].z++;
		}
		if (bullPos[i].z >= 30 + playerPos.z) {
			bullPos[i].z = -10;
			shot[i] = false;
		}
	}
	if (Remaining > 0) {
		if (Input::GetInstance()->PushKey(DIK_R)) {
			ReloadFlag = true;
		}
	}
	

	if (ReloadFlag == true) {
		ReloadTime += 1;
		ans = ReloadTime % 10;
		if (ans  == 0) {
			Remaining -= 1;
			if(Remaining == 0) {
				ReloadFlag = false;
				ReloadTime = 0;
			}
		}
	}
	
	

	SetPSR();
	AllUpdate();
}

void middle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for (int j = 0; j < 9; j++) {
		bull[j]->Draw();
	}
	for (int i = 0; i < 2; i++) {
		enemy[i]->Draw();
	}
	player->ObjDraw();
}

void middle::SpriteDraw()
{
	for (int i = Remaining; i < 8; i++) {
		bulletHUD[i]->Draw();
	}

	if (Remaining == 8) {
		Reload->Draw();
	}

	if (patern == 0) {
		changecount[0]->Draw();
	}
	else if (patern == 1) {
		changecount[1]->Draw();
	}
	else if (patern == 2) {
		changecount[2]->Draw();
	}
	else if (patern == 3) {
		changecount[3]->Draw();
	}
	else if (patern == 4) {
		changecount[4]->Draw();
	}

	maxcount->Draw();

	wave->Draw();
	slash->Draw();
		//bulletHUD[i]->Draw();

}

void middle::ImGuiDraw()
{
	player->ImGuiDraw();
	float r = Remaining;
	float p = patern;
	float a = ans;
	float t = ReloadTime;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("mouth");
	ImGui::Checkbox("shot", &shot[0]);
	ImGui::Checkbox("stop", &stop[0]);
	ImGui::Checkbox("stop", &stop[1]);
	ImGui::SliderFloat("ReloadTimer", &t, -100.0f, 100.0f);
	ImGui::SliderFloat("Remaining", &r, -100.0f, 100.0f);
	ImGui::SliderFloat("Remaining", &a, -100.0f, 100.0f);
	if (ImGui::TreeNode("playerPos")) {
		ImGui::SliderFloat("playerPos.z", &enemyPos[0].z, -100.0f, 100.0f);
		ImGui::SliderFloat("playerPos.z", &enemyPos[1].z, -100.0f, 100.0f);
		ImGui::SliderFloat("playerPos.z", &playerPos.z, -100.0f, 100.0f);
		ImGui::SliderFloat("playerPos.x", &playerPos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Pos.y", &bullPos[0].x, -100.0f, 100.0f);
		ImGui::SliderFloat("Pos.y", &bullPos[1].x, -100.0f, 100.0f);
		ImGui::SliderFloat("Pos.y", &bullPos[2].x, -100.0f, 100.0f);
		ImGui::SliderFloat("Pos.y", &bullPos[3].x, -100.0f, 100.0f);
		ImGui::SliderFloat("Pos.y", &bullPos[4].x, -100.0f, 100.0f);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("bulletHUD")) {
		ImGui::Checkbox("shot", &shot[0]);
		ImGui::Checkbox("shot", &shot[1]);
		ImGui::Checkbox("shot", &shot[2]);
		ImGui::Checkbox("shot", &shot[3]);
		ImGui::Checkbox("shot", &shot[4]);
		ImGui::Checkbox("shot", &shot[5]);
		ImGui::Checkbox("shot", &shot[6]);
		ImGui::Checkbox("shot", &shot[7]);
		ImGui::Checkbox("shot", &shot[8]);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("other")) {
		ImGui::SliderFloat("speed", &speed[0], -100.0f, 100.0f);
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
