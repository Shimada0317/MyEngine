#include "Actor.h"
#include "imgui.h"
#include"imconfig.h"
#include<fstream>
Actor::~Actor()
{
	player.reset();
	delete Reload;
	delete wave;
	delete slash;
	delete HpBer;
	for (int i = 0; i < 5; i++) {
		delete changecount[i];
		delete LifeCount[i];
	}
}

void Actor::Initialize()
{
	Sprite::LoadTexture(10, L"Resources/reload.png");
	Sprite::LoadTexture(13, L"Resources/one.png");
	Sprite::LoadTexture(14, L"Resources/two.png");
	Sprite::LoadTexture(15, L"Resources/three.png");
	Sprite::LoadTexture(16, L"Resources/four.png");
	Sprite::LoadTexture(17, L"Resources/five.png");
	Sprite::LoadTexture(18, L"Resources/Hpber.png");

	camera = new Camera(WinApp::window_width, WinApp::window_height);

	player = std::make_unique<Player>();
	player->Initalize(camera);
	camera->RecalculationMatrix();
	playerPos = player->GetPosition();

	//スプライトの読み込み
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

	heriM = ObjModel::CreateFromOBJ("heri", true);
	heri = Object3d::Create(heriM);

	haneM = ObjModel::CreateFromOBJ("hane", true);
	hane = Object3d::Create(haneM);

	Goal = Object3d::Create(heriM);

	playerMat = player->GetMat();
	playerHp = player->GetHp();

	getCamWorkF = player->GetCamWork();

	clearT = 0;

	heriFry = new Audio();
	heriFry->Initialize();
	heriFry->LoadFile("Resources/Sound/SE/heriFry.wav", 0.8f);
}

void Actor::SetPSR()
{
	playerMat = player->GetMat();

	//HUDのポジションセット
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ spSiz });
		bulletHUD[i]->SetPosition({ spPos.x,spPos.y + 32 * i });
	}
	//リロードの文字
	Reload->SetSize({ 128,64 });
	Reload->SetPosition({ 1140,300 });
	//左下のwaveの文字
	wave->SetSize({ 256,128 });
	wave->SetPosition({ 0,600 });
	//waveの最大数
	maxcount->SetSize({ 80,80 });
	maxcount->SetPosition({ 320, 630 });
	//waveの最大値と数字の間の/←これ
	slash->SetSize({ 80,80 });
	slash->SetPosition({ 280,630 });
	//変動するカウンター
	for (int i = 0; i < 5; i++) {
		changecount[i]->SetSize({ 80,80 });
		changecount[i]->SetPosition({ 240,630 });
		LifeCount[i]->SetSize({ 80,80 });
		LifeCount[i]->SetPosition({ 1200,630 });
	}
	//Hpバー
	HpBer->SetSize({ 128,64 });
	HpBer->SetPosition({ 1070,650 });
	player->SetHp(playerHp);
	playerPos = player->GetPosition();

	heri->SetPosition(heripos);
	heri->SetScale(heriscl);
	heri->SetRotation({0.0f,180.0f,0.0f});

	hane->SetRotation({ 0.0f,heriY,0.0f });
	if (startmove == false) {
		hane->SetPosition(heripos);
		hane->SetScale(heriscl);
	}
	else {
		hane->SetPosition(GoalPos);
		hane->SetScale(GoalScl);
	}

	Goal->SetPosition(GoalPos);
	Goal->SetScale(GoalScl);
	Goal->SetRotation({ 0.0f,90.0f,0.0f });

	heri->Update({ 0.7f,0.7f,0.6f,1.0f });
	Goal->Update({ 0.7f,0.7f,0.6f,1.0f });
	hane->Update({ 0.0f,0.0f,0.0f,1.0f });
}

void Actor::Update()
{
	float playeroldjup = playerHp;
	if (playeroldjup > playerHp) {
		
	}


	CountDistance = 0;
	heripos.m128_f32[2] += heriX;

	if (heripos.m128_f32[2] >= 20) {
		backObj = false;
		startmove = true;
	}
	else {
		heriY += 15.0f;
	}

	getCamWorkF = player->GetCamWork();
	if (getCamWorkF == true) {

		Robot.remove_if([](std::unique_ptr<Enemy>& robot) {
			return robot->IsDead();
			});

		if (Robot.empty()) {
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
	}
	
	if (patern >= 5) {
		heriY += 15.0f;
	}

	if (patern >= 6) {
		bool fring = player->GetFring();
		if (fring == true) {
			GoalPos.m128_f32[1] += 0.2f;
		}
	}
	XMFLOAT2 Player2DPos = player->GetRetPosition();
	bool PlayerBulletShot_F = player->GetBulletShot();
	//敵の更新処理
	for (std::unique_ptr<Enemy>& Enemy : Robot) {
		enemyPos[CountDistance] = Enemy->GetPosition();
		ene2DPos[CountDistance] = Enemy->Get2DPosition();
		for (int i = 0; i < 9; i++) {
			Enemy->Update(Player2DPos, playerHp, PlayerBulletShot_F);
		}
		CountDistance += 1;
	}
	player->SetBulletShot(PlayerBulletShot_F);
	player->PlayerMove(move, patern);
	//座標の設定
	SetPSR();
	//プレイヤーの更新処理
	player->Update(Remaining,camera,patern);
	camera->RecalculationMatrix();
	
}

void Actor::Draw(DirectXCommon* dxCommon)
{
	
	Object3d::PreDraw(dxCommon->GetCmdList());
	Goal->Draw();
	hane->Draw();
	if (backObj == true) {
		heri->Draw();
	}
	player->ObjDraw();
	Object3d::PostDraw();
	for (std::unique_ptr<Enemy>& robot : Robot) {
		robot->Draw(dxCommon);
	}
	player->ParticleDraw(dxCommon->GetCmdList());
}

void Actor::SpriteDraw()
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

void Actor::ImGuiDraw()
{

	/*for (std::unique_ptr<Enemy>& robot : Robot) {
		robot->ImgDraw();
	}
	player->ImGuiDraw();*/
	//float a = patern;
	//ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	//ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	//ImGui::Begin("Enemy");
	//ImGui::SliderFloat("EnemyPos x", &enemyPos[0].m128_f32[0], -100.0f, 100.0f);
	//ImGui::SliderFloat("EnemyPos y", &enemyPos[0].m128_f32[1], -100.0f, 100.0f);
	//ImGui::SliderFloat("EnemyPos z", &enemyPos[0].m128_f32[2], -100.0f, 100.0f);

	//ImGui::SliderFloat("EnemyPos x", &enemyPos[1].m128_f32[0], -100.0f, 100.0f);
	//ImGui::SliderFloat("EnemyPos y", &enemyPos[1].m128_f32[1], -100.0f, 100.0f);
	//ImGui::SliderFloat("EnemyPos z", &enemyPos[1].m128_f32[2], -100.0f, 100.0f);

	//ImGui::SliderFloat("EnemyPos x", &enemyPos[2].m128_f32[0], -100.0f, 100.0f);
	//ImGui::SliderFloat("EnemyPos y", &enemyPos[2].m128_f32[1], -100.0f, 100.0f);
	//ImGui::SliderFloat("EnemyPos z", &enemyPos[2].m128_f32[2], -100.0f, 100.0f);

	//ImGui::End();
	//ImGui::PopStyleColor();
	//ImGui::PopStyleColor();
}


	
void Actor::LoadEnemyPopData()
{
	std::ifstream file;
	file.open("Resources/LoadEnemy.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();


	file.close();
}

void Actor::UpdataEnemyPopCommands()
{
	//待機処理
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
	bool step = false;
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
			//WAVEの要素
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
			//座標取得
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
			//追尾先の座標取得
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

			else if (word.find("STEP") == 0) {
				getline(line_stram, word, ',');
				int STEP = atoi(word.c_str());
				if (STEP == 1) {
					step = true;
				}
				else {
					step = false;
				}

			}

			//
			else if (word.find("ARIVE") == 0) {
				getline(line_stram, word, ',');

				//待ち時間
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
				std::unique_ptr<Enemy> newRobot = std::make_unique<Enemy>();
				newRobot->Initialize(ROTATION,POSITION,camera, step);
				newRobot->SetTrackPoint(TRACK);
				Robot.push_back(std::move(newRobot));

				POPSkip = false;
				TRACKSkip = false;
				ARIVESkip = false;
			}
		}

		if (patern < count) {
			break;
		}

		if (word.find("END") == 0) {
			getline(line_stram, word, ',');

			break;
		}

	}
}