#include "Actor.h"
#include "imgui.h"
#include"imconfig.h"
#include"ModelManager.h"
#include<fstream>

Actor::~Actor()
{
	player.reset();
	delete Reload;
	delete Wave;
	delete Slash;
	delete HpBer;
	for (int i = 0; i < 5; i++) {
		delete ChangeCount[i];
		delete LifeCount[i];
	}
}

void Actor::Initialize()
{
	Sprite::LoadTexture(10, L"Resources/Reload.png");
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

	//スプライトの読み込み
	for (int i = 0; i < 9; i++) {
		Sprite::LoadTexture(i, L"Resources/bullet.png");
		SpritePos[i] = { 1220.0f,25.0f + 32.0f * i };
		SpriteRot[i] = 0;
		Time[i] = 0;
		bulletHUD[i] = Sprite::SpriteCreate(i, SpritePos[i], {1.0f,1.0f,1.0f,1.0f},AnchorPoint);
		DropBullet[i] = false;
	}

	Reload = Sprite::SpriteCreate(10, ReloadSpritePos, ReloadSpriteColor, AnchorPoint);
	Wave = Sprite::SpriteCreate(11, { 10.0f,10.0f });
	Slash = Sprite::SpriteCreate(12, { 10.0f,10.0f });
	MaxCount = Sprite::SpriteCreate(17, { 10.0f,10.0f });
	HpBer = Sprite::SpriteCreate(18, { 10.0f,10.0f });
	for (int i = 0; i < 5; i++) {
		ChangeCount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	for (int i = 0; i < 5; i++) {
		LifeCount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	LoadEnemyPopData();

	heri = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	Goal = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	hane = Object3d::Create(ModelManager::GetInstance()->GetModel(12));

	PlayerHp = player->GetHp();
	GetCamWork_F = player->GetCamWork();
	OldRemaining = Remaining;

	heriFry = new Audio();
	heriFry->Initialize();
	heriFry->LoadFile("Resources/Sound/SE/heriFry.wav", 0.8f);
}

void Actor::SetPSR()
{
	if (OldRemaining < Remaining) {
		DropBullet[OldRemaining] = true;
		OldRemaining = Remaining;
	}

	for (int i = 0; i < 8; i++) {
		if (DropBullet[i] == true&&SpritePos[i].y <= 1300) {
			SpritePos[i].y += 10;
			Time[i] += 0.5f;
			Action::GetInstance()->ThrowUp(Gravity,Time[i], 40, SpritePos[i].y);
			SpriteRot[i] += 80;
		}
		else if (SpritePos[i].y > 1300) {
			Time[i] = 0;
		}
	}

	//HUDのポジションセット
	for (int i = 0; i < 9; i++) {
		bulletHUD[i]->SetSize({ SpriteSiz });
		bulletHUD[i]->SetPosition(SpritePos[i]);
		bulletHUD[i]->SetRotation(SpriteRot[i]);
	}
	//リロードの文字
	Reload->SetSize(ReloadSpriteSize);
	//左下のwaveの文字
	Wave->SetSize({ 256,128 });
	Wave->SetPosition({ 0,600 });
	//waveの最大数
	MaxCount->SetSize({ 80,80 });
	MaxCount->SetPosition({ 320, 630 });
	//waveの最大値と数字の間の/←これ
	Slash->SetSize({ 80,80 });
	Slash->SetPosition({ 280,630 });
	//変動するカウンター
	for (int i = 0; i < 5; i++) {
		ChangeCount[i]->SetSize({ 80,80 });
		ChangeCount[i]->SetPosition({ 240,630 });
		LifeCount[i]->SetSize({ 80,80 });
		LifeCount[i]->SetPosition({ 1200,630 });
	}
	//Hpバー
	HpBer->SetSize({ 128,64 });
	HpBer->SetPosition({ 1070,650 });
	player->SetHp(PlayerHp);

	heri->SetPosition(Heripos);
	heri->SetScale(Heriscl);
	heri->SetRotation({ 0.0f,180.0f,0.0f });

	hane->SetRotation({ 0.0f,HeriY,0.0f });
	if (StartMovie == false) {
		hane->SetPosition(Heripos);
		hane->SetScale(Heriscl);
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

	Reload_F = player->GetReload();
}

void Actor::Update()
{
	float playeroldjup = PlayerHp;

	Heripos.m128_f32[2] += HeriX;

	if (Heripos.m128_f32[2] >= 20) {
		BackObj = false;
		StartMovie = true;
	}
	else {
		HeriY += 15.0f;
	}

	GetCamWork_F = player->GetCamWork();
	if (GetCamWork_F == true) {
		Robot.remove_if([](std::unique_ptr<Enemy>& robot) {
			return robot->IsDead();
			});
		if (Robot.empty()) {
			Move = true;
		}
		finish = player->GetFinish();
		if (finish == true) {
			Move = false;
		}
		if (Move == false && finish == true) {
			UpdataEnemyPopCommands();
			patern += 1;
			finish = false;
			player->SetFinish(finish);
		}
	}

	if (patern >= 5) {
		HeriY += 15.0f;
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
		Enemy->Update(Player2DPos, PlayerHp, PlayerBulletShot_F);
	}
	player->SetBulletShot(PlayerBulletShot_F);
	player->PlayerMove(Move, patern);
	//座標の設定
	SetPSR();
	//プレイヤーの更新処理
	player->Update(Remaining, camera, patern);
	if (Remaining >= 8) {
		if (Revers == false) {
			Action::GetInstance()->EaseOut(ReloadSpriteSize.x, 210);
			Action::GetInstance()->EaseOut(ReloadSpriteSize.y, 140);
			if (ReloadSpriteSize.x >= 200) {
				Revers = true;
			}
		}
		else {
			Action::GetInstance()->EaseOut(ReloadSpriteSize.x, 150);
			Action::GetInstance()->EaseOut(ReloadSpriteSize.y, 80);
			if (ReloadSpriteSize.x <= 160) {
				Revers = false;
			}
		}
	}
	else if (Remaining == 0) {
		for (int i = 0; i < 9; i++) {
			SpritePos[i] = { 1220.0f,25.0f + 32.0f * i };
			SpriteRot[i] = 0;
			Time[i] = 0;
			DropBullet[i] = false;
			OldRemaining = Remaining;
		}
	}

	camera->RecalculationMatrix();
}

void Actor::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	Goal->Draw();
	hane->Draw();
	if (BackObj == true) {
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
	if (GetCamWork_F == true) {
		for (int i = 0; i < 8; i++) {
			if (Remaining <= 8&&Reload_F==false) {
				bulletHUD[i]->Draw();
			}
		}
		if (Remaining >= 8) {
			Reload->Draw(ReloadSpriteColor);
		}

		if (PlayerHp == 1) {
			LifeCount[0]->Draw();
		}
		else if (PlayerHp == 2) {
			LifeCount[1]->Draw();
		}
		else if (PlayerHp == 3) {
			LifeCount[2]->Draw();
		}
		else if (PlayerHp == 4) {
			LifeCount[3]->Draw();
		}
		else if (PlayerHp == 5) {
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
	}*/
	player->ImGuiDraw();
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

	EnemyPopCommands << file.rdbuf();

	file.close();
}

void Actor::UpdataEnemyPopCommands()
{
	//待機処理
	if (Wait_F == true) {
		WaitT--;
		if (WaitT <= 0) {
			Wait_F = false;
		}
		return;
	}

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

	while (getline(EnemyPopCommands, line))
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
				newRobot->Initialize(ROTATION, POSITION, camera, step);
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