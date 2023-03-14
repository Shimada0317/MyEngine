#include "Actor.h"
#include "imgui.h"
#include"imconfig.h"
#include"ModelManager.h"
#include<fstream>


bool otherenemyarive = true;

Actor::~Actor()
{
	player.reset();
	delete Wave;
	delete HpBer;
	for (int i = 0; i < 5; i++) {
		delete ChangeCount[i];
		delete LifeCount[i];
	}
}

void Actor::Initialize()
{

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

	Wave = Sprite::SpriteCreate(11, { 10.0f,10.0f });
	MaxCount = Sprite::SpriteCreate(17, { 10.0f,10.0f });
	HpBer = Sprite::SpriteCreate(18, { 10.0f,10.0f });


	for (int i = 0; i < 5; i++) {
		ChangeCount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	for (int i = 0; i < 5; i++) {
		LifeCount[i] = Sprite::SpriteCreate(13 + i, { 10.0f,10.0f });
	}
	LoadEnemyPopData();

	Heri = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	Goal = Object3d::Create(ModelManager::GetInstance()->GetModel(11));
	Hane = Object3d::Create(ModelManager::GetInstance()->GetModel(12));

	PlayerHp = player->GetHp();
	GetCamWorkFlag = player->GetCamWork();


	heriFry = new Audio();
	heriFry->Initialize();
	heriFry->LoadFile("Resources/Sound/SE/heriFry.wav", 0.8f);
}

void Actor::SetPSR()
{

	//左下のwaveの文字
	Wave->SetSize({ 256,128 });
	Wave->SetPosition({ 0,600 });
	//waveの最大数
	MaxCount->SetSize({ 80,80 });
	MaxCount->SetPosition({ 320, 630 });
	//変動するカウンター
	for (int i = 0; i < 5; i++) {
		ChangeCount[i]->SetSize({ 80,80 });
		ChangeCount[i]->SetPosition({ 240,630 });
		LifeCount[i]->SetSize({ 80,80 });
		LifeCount[i]->SetPosition({ 1160,630 });
	}
	//Hpバー
	HpBer->SetSize({ 224,96 });
	HpBer->SetPosition({ WinApp::window_width - 228,WinApp::window_height - 100 });
	player->SetHp(PlayerHp);

	Heri->SetPosition(Heripos);
	Heri->SetScale(Heriscl);
	Heri->SetRotation({ 0.0f,180.0f,0.0f });

	Hane->SetRotation({ 0.0f,HeriY,0.0f });
	if (StartMovieFlag == false) {
		Hane->SetPosition(Heripos);
		Hane->SetScale(Heriscl);
	}
	else {
		Hane->SetPosition(GoalPos);
		Hane->SetScale(GoalScl);
	}

	Goal->SetPosition(GoalPos);
	Goal->SetScale(GoalScl);
	Goal->SetRotation({ 0.0f,270.0f,0.0f });

	Heri->Update({ 0.7f,0.7f,0.6f,1.0f });
	Goal->Update({ 0.7f,0.7f,0.6f,1.0f });
	Hane->Update({ 0.0f,0.0f,0.0f,1.0f });
}

void Actor::Update()
{
	XMVECTOR velo = player->GetVelocity();
	SetVelocity(velo);

	Heripos.m128_f32[2] += HeriX;

	if (Heripos.m128_f32[2] >= 20) {
		BackObjFlag = false;
		StartMovieFlag = true;
	}
	else {
		HeriY += 15.0f;
	}

	GetCamWorkFlag = player->GetCamWork();
	if (GetCamWorkFlag == true) {
		Robot.remove_if([](std::unique_ptr<Enemy>& robot) {
			return robot->IsDead();
			});
		if (Robot.empty()) {
			MoveFlag = true;
		}
		StopFlag = player->GetFinish();
		if (StopFlag == true) {
			MoveFlag = false;
		}
		if (MoveFlag == false && StopFlag == true) {
			UpdataEnemyPopCommands();
			Patern += 1;
			StopFlag = false;
			player->SetFinish(StopFlag);
		}
	}

	if (Patern >= 5) {
		HeriY += 15.0f;
	}

	if (Patern >= 6) {
		bool fring = player->GetFring();
		if (fring == true) {
			GoalPos.m128_f32[1] += velo.m128_f32[1];
		}
	}
	
	XMFLOAT2 Player2DPos = player->GetRetPosition();
	bool PlayerBulletShot_F = player->GetBulletShot();
	//敵の更新処理
	for (std::unique_ptr<Enemy>& Enemy : Robot) {
		Enemy->Update(Player2DPos, PlayerHp, PlayerBulletShot_F);
	}
	CheckSameTrackPosition();
	player->SetBulletShot(PlayerBulletShot_F);
	player->PlayerMove(MoveFlag, Patern);
	//座標の設定
	SetPSR();
	//プレイヤーの更新処理
	player->Update(camera, Patern);


	camera->RecalculationMatrix();
}

void Actor::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCmdList());
	Goal->Draw();
	Hane->Draw();
	if (BackObjFlag == true) {
		Heri->Draw();
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
	if (GetCamWorkFlag == true) {

		HpBer->Draw();

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
	}
	player->SpriteDraw();
}

void Actor::ImGuiDraw()
{

	/*for (std::unique_ptr<Enemy>& robot : Robot) {
		robot->ImgDraw();
	}*/
	player->ImGuiDraw();
	float a = Patern;
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Patern");
	ImGui::SliderFloat("Patern", &a, 0.0f, 10.0f);



	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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
	if (WaitFlag == true) {
		WaitT--;
		if (WaitT <= 0) {
			WaitFlag = false;
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
		if (Patern == count) {
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
				newRobot->Initialize(ROTATION, POSITION, camera,TRACK ,step);
				Robot.push_back(std::move(newRobot));

				POPSkip = false;
				TRACKSkip = false;
				ARIVESkip = false;
			}
		}

		if (Patern < count) {
			break;
		}

		if (word.find("END") == 0) {
			getline(line_stram, word, ',');

			break;
		}

	}
}

void Actor::CheckSameTrackPosition()
{
	for (std::unique_ptr<Enemy>& FirstEnemy : Robot) {
		for (std::unique_ptr<Enemy>& SecondEnemy : Robot) {
			if (FirstEnemy.get() != SecondEnemy.get()) {
				XMVECTOR FirstTrackPosition = FirstEnemy->GetTrackPos();
				XMVECTOR SecondTrackPosition = SecondEnemy->GetTrackPos();
				if (Action::GetInstance()->CompletelyTogetherXMVECTOR(FirstTrackPosition, SecondTrackPosition)) {
					bool secondenemyarive = SecondEnemy->GetArive();
					bool CountZero = SecondEnemy->GetCountZero();
					if (secondenemyarive == true) {
						otherenemyarive = true;
					}
					else if(secondenemyarive==false||CountZero == true)
					{
						otherenemyarive = false;
					}
					SecondEnemy->WaitTrack(otherenemyarive);
				}
			}
		}

	}
	

}
