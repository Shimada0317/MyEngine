#include "middle.h"
#include"imgui/imgui.h"

void middle::Initialize()
{
	//�v���C���[(���e�B�N��)�̓ǂݍ���
	player = std::make_unique<Player>();
	player->Initalize();
	playerPos = player->GetPosition();

	//�e�̓ǂݍ���
	for (int j = 0; j < 9; j++) {
		bull[j] = std::make_unique<Bullet>();
		bull[j]->Initialize();

		retime[j] = true;
		bullPos[j] = bull[j]->GetPosition();
		bullScl = bull[j]->GetScl();
		lost = bull[j]->GetLost();
		debug[j] = 0;
		speed[j] = 0.5f;
	}
	//�G�̓ǂݍ���
	for (int i = 0; i < 2; i++) {
		enemy[i] = std::make_unique<Enemy>();
		enemy[i]->Initalize();
		enemyPos[i] = enemy[i]->GetPosition();
		enemyScl = enemy[i]->GetScl();
		life[i] = enemy[i]->GetLife();
		stop[i] = false;
	}

	////�X�v���C�g�̓ǂݍ���
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
	//HUD�̃|�W�V�����Z�b�g
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ spSiz });
		bulletHUD[i]->SetPosition({ spPos.x,spPos.y + 32 * i });
	}
	//�����[�h�̕���
	Reload->SetSize({ 128,64 });
	Reload->SetPosition({ 1140,300 });
	//������wave�̕���
	wave->SetSize({ 256,128 });
	wave->SetPosition({ 0,600 });
	//wave�̍ő吔
	maxcount->SetSize({ 80,80 });
	maxcount->SetPosition({ 320, 630 });
	//wave�̍ő�l�Ɛ����̊Ԃ�/������
	slash->SetSize({ 80,80 });
	slash->SetPosition({ 280,630 });
	//�ϓ�����J�E���^�[
	for (int i = 0; i < 5; i++) {
		changecount[i]->SetSize({ 80,80 });
		changecount[i]->SetPosition({ 240,630 });
	}

	//�v���C���[�̃|�W�V�����Z�b�g
	player->SetPosition(playerPos);
	//�e�̃|�W�V�����Z�b�g
	for (int j = 0; j < 9; j++) {
		bull[j]->SetPosition(bullPos[j]);
		bull[j]->SetScl(bullScl);
		bull[j]->SetLost(lost);
	}
	//�G�̃|�W�V�����Z�b�g
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
	//�v���C���[�̍X�V
	player->Update();

	//�G�̍X�V
	for (int i = 0; i < 2; i++) {
		enemy[i]->Update(playerPos);
		enemyPos[i] = enemy[i]->GetPosition();
	}
	//�e�̍X�V
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
	//�G��|�������Ahit�J�E���g���グ��
	if (count == true) {
		hit += 1;
		count = false;
	}

	//hit�J�E���g��2�ɂȂ������A�E�F�[�u��i�߂�
	if (hit == 2) {
		patern += 1;
		cammove = 0.1f;
		hit = 0;
	}
	//wave���i�ނ��ƂɃJ���������ɐi��
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
			/*for (int i = 0; i < 9; i++) {
				if (shot[i] == false) {
					bullPos[i].x = playerPos.x;
					bullPos[i].y = playerPos.y;
					bullPos[i].z = playerPos.z;
					shot[i] = true;
					break;
				}
			}*/

			for (int i = 0; i < 9; i++) {
				if (shot[i] == false) {
					bullPos[i].x = startPos.x;
					bullPos[i].y = startPos.y;
					bullPos[i].z = startPos.z;
					shot[i] = true;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 9; i++) {
		if (shot[i] == true) {
			float vx = (startPos.x - playerPos.x);
			float vy = (startPos.y - playerPos.y);
			float vz = (startPos.z - playerPos.z);
			float v2x = pow(vx, 2);
			float v2y = pow(vy, 2);
			float v2z = pow(vz, 2);
			float l = sqrtf(v2x + v2y+v2z);
			float v3x = (vx / l) * speedm;
			float v3y = (vy / l) * speedm;
			float v3z = (vz / l) * speedm;

			bullPos[i].x -= v3x;
			bullPos[i].y -= v3y;
			bullPos[i].z -= v3z;

		/*	verosity_ = { 0, 0, bullSpeed, 1 };
			bullPos[i].z += verosity_.m128_f32[2];*/
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
	delete[] bulletHUD;
}
