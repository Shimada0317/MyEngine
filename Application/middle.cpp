#include "middle.h"
#include "imgui/imgui.h"
#include"imgui/imconfig.h"
#include<fstream>
middle::~middle()
{
	for (int i = 0; i < 9; i++) {
		delete bull[i];
	}
}
//
////�Z�[�u
//
void middle::Initialize()
{
	Sprite::LoadTexture(10, L"Resources/reload.png");
	Sprite::LoadTexture(11, L"Resources/wave.png");
	Sprite::LoadTexture(12, L"Resources/slash.png");
	Sprite::LoadTexture(13, L"Resources/one.png");
	Sprite::LoadTexture(14, L"Resources/two.png");
	Sprite::LoadTexture(15, L"Resources/three.png");
	Sprite::LoadTexture(16, L"Resources/four.png");
	Sprite::LoadTexture(17, L"Resources/five.png");
	Sprite::LoadTexture(18, L"Resources/Hpber.png");
	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);

	player = std::make_unique<Player>();
	player->Initalize();

	playerPos = player->GetPosition();

	for (int i = 0; i < 9; i++) {
		bull[i] = new Bullet();
		bull[i]->Initialize();
	}


	////�X�v���C�g�̓ǂݍ���
	for (int i = 0; i < 9; i++) {
		Sprite::LoadTexture(i, L"Resources/bullet.png");
		bulletHUD[i] = Sprite::SpriteCreate(i, { 10.0f,10.0f });
	}

	Reload = Sprite::SpriteCreate(10, { 10.0f,10.0f }, { 1.0f,1.0f,1.0f,1.0f });
	wave = Sprite::SpriteCreate(11, { 10.0f,10.0f });
	slash = Sprite::SpriteCreate(12, { 10.0f,10.0f });
	maxcount = Sprite::SpriteCreate(17, { 10.0f,10.0f });
	HpBer = Sprite::SpriteCreate(18, { 10.0f,10.0f });
	for (int i = 0; i < 5; i++) {
		changecount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	for (int i = 0; i < 5; i++) {
		LifeCount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}

	LoadEnemyPopData();
	//UpdataEnemyPopCommands();

	playerMat = player->GetMat();
	playerHp = player->GetHp();

	getCamWorkF = player->GetCamWork();
}

void middle::SetPSR()
{
	
	playerMat = player->GetMat();

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
		LifeCount[i]->SetSize({ 80,80 });
		LifeCount[i]->SetPosition({ 1200,630 });
	}
	//Hp�o�[
	HpBer->SetSize({ 128,64 });
	HpBer->SetPosition({ 1070,650 });
	player->SetHp(playerHp);
	playerPos = player->GetPosition();

	/*for (int i = 0; i < MAXENEMY; i++) {
		rob[i]->SetRotation({ enemyRot[i] });
	}*/
}

void middle::Updata()
{

	getCamWorkF = player->GetCamWork();
	if (getCamWorkF == true) {

		rob.remove_if([](std::unique_ptr<Robot>& robot) {
			return robot->IsDead();
			});

		if (rob.empty()) {
			move = true;
		}

		finish = player->GetFinish();
		if (finish == true) {
			move = false;
		}
		if (move == false && finish == true) {
			UpdataEnemyPopCommands();
			patern += 1;
			finish = false;
			player->SetFinish(finish);
		}



		//�G�̍X�V����
		for (std::unique_ptr<Robot>& Robot : rob) {
			for (int i = 0; i < 9; i++) {
				Robot->Updata(bull[i], playerMat, spown, playerHp);
			}
		}

	}

	player->PlayerMove(move, patern);
	//���W�̐ݒ�
	SetPSR();
	//�v���C���[�̍X�V����
	player->Updata(bull, Remaining);
}

void middle::Draw(DirectXCommon* dxCommon)
{
	for (std::unique_ptr<Robot>& robot : rob) {
		robot->Draw(dxCommon);
	}
	player->ParticleDraw(dxCommon->GetCmdList());
	Object3d::PreDraw(dxCommon->GetCmdList());
	for (int i = 0; i < 9; i++) {
		bull[i]->Draw();
	}
	player->ObjDraw();
	Object3d::PostDraw();

}
//
void middle::SpriteDraw()
{
	if (getCamWorkF == true) {

		for (int i = Remaining; i < 8; i++) {
			bulletHUD[i]->Draw();
		}

		if (Remaining == 8) {
			Reload->Draw();
		}

		if (playerHp == 1) {
			LifeCount[0]->Draw();
		}
		else if (playerHp == 2) {
			LifeCount[1]->Draw();
		}
		else if (playerHp == 3) {
			LifeCount[2]->Draw();
		}
		else if (playerHp == 4) {
			LifeCount[3]->Draw();
		}
		else if (playerHp == 5) {
			LifeCount[4]->Draw();
		}
		HpBer->Draw();
	}

	player->SpriteDraw();
}
//
void middle::ImGuiDraw()
{
	player->ImGuiDraw();
	float a = patern;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Plyer");
	ImGui::Checkbox("move", &move);
	ImGui::Checkbox("finish", &finish);
	ImGui::SliderFloat("patern", &a, -100.0f, 100.0f);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

}

void middle::Fainalize()
{

	player->Finalize();
	//delete[] bulletHUD;
}

	
void middle::LoadEnemyPopData()
{
	std::ifstream file;
	file.open("Resources/LoadEnemy.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();


	file.close();
}

void middle::UpdataEnemyPopCommands()
{
	//�ҋ@����
	if (waitF == true) {
		waitT--;
		if (waitT <= 0) {
			waitF = false;
		}
		return;
	}
	ENE = 0;
	std::string line;

	XMVECTOR POSITION = { 0.0f,0.0f,0.0f };
	XMVECTOR TRACK = { 0.0f,0.0f,0.0f };
	XMFLOAT3 ROTATION = { 0.0f,0.0f,0.0f };
	bool ari = false;
	int count = 0;

	bool POPSkip = false;
	bool TRACKSkip = false;
	bool ARIVESkip = false;

	while (getline(enemyPopCommands, line))
	{

		std::istringstream line_stram(line);

		std::string word;

		getline(line_stram, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("WAVE") == 0) {
			getline(line_stram, word, ',');

			//WAVE�̗v�f
			count = atoi(word.c_str());
		}
		if (patern == count) {
			if (word.find("ROTATION") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				ROTATION.x = x;
				ROTATION.y = y;
				ROTATION.z = z;

				POPSkip = true;
			}
			//���W�擾
			else if (word.find("POSITION") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				POSITION.m128_f32[0] = x;
				POSITION.m128_f32[1] = y;
				POSITION.m128_f32[2] = z;

				POPSkip = true;
			}
			//�ǔ���̍��W�擾
			else if (word.find("TRACK") == 0) {

				getline(line_stram, word, ',');
				float x = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float y = (float)std::atof(word.c_str());

				getline(line_stram, word, ',');
				float z = (float)std::atof(word.c_str());

				TRACK.m128_f32[0] = x;
				TRACK.m128_f32[1] = y;
				TRACK.m128_f32[2] = z;

				TRACKSkip = true;
			}

			//
			else if (word.find("ARIVE") == 0) {
				getline(line_stram, word, ',');

				//�҂�����
				int Arive = atoi(word.c_str());
				if (Arive == 1) {
					ari = true;
				}
				else {
					ari = false;
				}

				ARIVESkip = true;
			}

			if (ARIVESkip == true && POPSkip == true && TRACKSkip == true) {
				std::unique_ptr<Robot> newRobot = std::make_unique<Robot>();
				newRobot->Initialize(ROTATION);
				newRobot->SetPosition(POSITION);
				newRobot->SetTrackPoint(TRACK);
				//newRobot->SetRotation(ROTATION);
				rob.push_back(std::move(newRobot));

				POPSkip = false;
				TRACKSkip = false;
				ARIVESkip = false;
			}
		}

		if (patern < count) {
			//patern += 1;
			break;
		}

		if (word.find("END") == 0) {
			getline(line_stram, word, ',');

			break;
		}

	}
}
