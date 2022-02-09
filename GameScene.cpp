#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

void GameScene::StaticInitialize(DirectXCommon* dxCommon,WinApp* winApp,Input* input) {

	audio = new Audio();
	audio->StaticInitialize();
	se = new Audio();
	se->StaticInitialize();
	hit = new Audio();
	hit->StaticInitialize();


	input->Initialize(winApp);
}

void GameScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);

	this->dxCommon = dxCommon;
	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/haikei.png");
	Sprite::LoadTexture(2, L"Resources/life.png");
	Sprite::LoadTexture(3, L"Resources/title.png");
	Sprite::LoadTexture(4, L"Resources/clear.png");
	Sprite::LoadTexture(5, L"Resources/gameover.png");
	Sprite::LoadTexture(6, L"Resources/final.png");
	Sprite::LoadTexture(debugTextNumber, L"Resources/debugfont.png");

	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);

	spritetest = Sprite::SpriteCreate(1, { 1.0f,1.0f });
	life = Sprite::SpriteCreate(2, { 1.0f,1.0f });
	title = Sprite::SpriteCreate(3, { 1.0f,1.0f });
	clear = Sprite::SpriteCreate(4, { 1.0f,1.0f });
	gameover = Sprite::SpriteCreate(5, { 1.0f,1.0f });
	last = Sprite::SpriteCreate(6, { 1.0f,1.0f });


	//モデルの読み込み
	model = Model::LoadFromObJ("ramieru");
	playermodel = Model::LoadFromObJ("player");
	playermodel2 = Model::LoadFromObJ("filter1");
	playermodel3 = Model::LoadFromObJ("filter2");
	
	//オブジェクトのクリエイト
	player3d = Object3d::Create();
	player3d1 = Object3d::Create();
	player3d2 = Object3d::Create();
	
	
	//モデルのセット
	player3d->SetModel(playermodel);
	player3d1->SetModel(playermodel2);
	player3d2->SetModel(playermodel3);
	for (int i = 0; i < 20; i++) {
		block3d[i] = Object3d::Create();
		block3d[i]->SetModel(model);
	}
}


void GameScene::GameSceneInitialize()
{
	assert(audio);
	this->audio = audio;


	for (int i = 0; i < 20; i++) {
		block3d[i]->SetScale({ 0.5f,0.5f,0.5f });
		block3d[i]->SetRotation({ 90,0,0 });
		enemyspeed[i].x = (rand() % 6) - 3;
		enemyspeed[i].y = (rand() % 5) - 2;
		enemyspeed[i].z = (rand() % 50) + 15;
		//ブロックの最高高度1.3f,最低は-1.3f
		//ブロック画面端は絶対値3

	}
	//プレイヤー
	player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale(scale);
	//プレイヤー2
	player3d1->SetRotation({ 0,450,-270 });
	player3d1->SetPosition({ 0,0,0 });
	player3d1->SetScale(scale);
	//プレイヤー3
	player3d2->SetRotation({ 0,450,-270 });
	player3d2->SetPosition({ 0,0,0 });
	player3d2->SetScale(scale);
	//スプライトサイズ
	spritetest->SetSize({ WindowWidth,WindowHeight });
	title->SetSize({ WindowWidth,WindowHeight });
	clear->SetSize({ WindowWidth,WindowHeight });
	gameover->SetSize({ WindowWidth,WindowHeight });
	last->SetSize({ WindowWidth,WindowHeight });

	for (int i = 0; i < 20; i++) {
		enemyF[i] = 0;
	}

	vol[0] = 0.1f;
	vol[1] = 0.01f;
	vol[2] = 0.01f;

	audio->LoadFile("Resources/digitalworld.wav", vol[0]);
}

void GameScene::Update(DirectXCommon* dxCommon,Input* input)
{
	//DirectX毎フレーム処理 ここから
	this->input = input;
	this->dxCommon = dxCommon;
	//this->audio = audio;

	//audio->LoadFile("Resources/digitalworld.wav", vol[0]);
	
	if (scene == 0) {
		count = 0;
		checker = 0;
		background = 0;
		speed = { 0.0f,0.0f,0.0f };
		player3d->SetPosition(speed);
		player3d1->SetPosition(speed);
		player3d2->SetPosition(speed);
		for (int i = 0; i < 20; i++) {
			enemyF[i] = 0;
			enemyspeed[i].x = (rand() % 6) - 3;
			enemyspeed[i].y = (rand() % 5) - 2;
			enemyspeed[i].z = (rand() % 50) + 15;
			block3d[i]->SetPosition(enemyspeed[i]);
		}
		player3d->Update();
		player3d1->Update();
		player3d2->Update();
		if (input->PushKey(DIK_SPACE)) {
			scene = 1;
		}

	}
	if (scene == 1) {
		debugText->Print("Move:arrow",400, 0, 2.0f);
		debugText->Print("delete:space", 400, 30, 2.0f);
		checker++;
		if (input->PushKey(DIK_RIGHT) || input->PushKey(DIK_D)) {
			speed.x += 0.05f;
			player3d->SetPosition(speed);
			player3d1->SetPosition(speed);
			player3d2->SetPosition(speed);
			if (speed.x >= 4.1f) {
				speed.x = 4.1f;
			}
		}

		if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_A)) {
			speed.x -= 0.05f;
			player3d->SetPosition(speed);
			player3d1->SetPosition(speed);
			player3d2->SetPosition(speed);
			if (speed.x <= -4.1f) {
				speed.x = -4.1f;
			}
		}

		if (input->PushKey(DIK_UP) || input->PushKey(DIK_W)) {
			speed.y += 0.05f;
			player3d->SetPosition(speed);
			player3d1->SetPosition(speed);
			player3d2->SetPosition(speed);
			if (speed.y >= 1.9f) {
				speed.y = 1.9f;
			}
		}

		if (input->PushKey(DIK_DOWN) || input->PushKey(DIK_S)) {
			speed.y -= 0.05f;
			player3d->SetPosition(speed);
			player3d1->SetPosition(speed);
			player3d2->SetPosition(speed);
			if (speed.y <= -1.9f) {
				speed.y = -1.9f;
			}
		}

		if (moveAction == 1) {
			movetimer++;
		}

		if (movetimer >= 3 && movetimer <= 4) {
			move = 1;
		}

		else if (movetimer >= 5 && movetimer <= 6) {
			move = 2;
		}

		else if (movetimer >= 50) {
			moveAction = 0;
			movetimer = 0;
			move = 0;
		}

		for (int i = 0; i < 20; i++) {
			if (enemyF[i] == 0) {
				enemyradius[i] = 0.5f;
				a[i] = speed.x - enemyspeed[i].x;
				b[i] = speed.y - enemyspeed[i].y;
				c[i] = speed.z - enemyspeed[i].z;
				d[i] = sqrtf(a[i] * a[i] + b[i] * b[i] + c[i] * c[i]);
				if (d[i] <= playerradius + enemyradius[i]) {
					hit->LoadFile("Resources/coin4.wav", vol[2]);
				}

				if (move == 0) {
					if (input->PushKey(DIK_SPACE)) {
						if (d[i] <= playerradius + enemyradius[i]) {
							se->LoadFile("Resources/se_ymd04.wav", vol[0]);
							enemyF[i] = 1;
							count += 1;
						}
						moveAction = 1;
					}
				}
			}
		}


		for (int i = 0; i < 20; i++) {
			block3d[i]->Update();
		}

		for (int i = 0; i < 20; i++) {
			enemyspeed[i].z -= 0.05f;
			block3d[i]->SetPosition(enemyspeed[i]);

			if (enemyspeed[i].z <= -3.0f) {
				enemyspeed[i].x = (rand() % 6) - 3;
				enemyspeed[i].y = (rand() % 5) - 2;
				enemyspeed[i].z = (rand() % 25) + 15;
				break;
			}

		}

		if (count >= 20) {
			scene = 2;
		}
		if (checker >= 2000) {
			background = 1;
		}

		if (checker >= 4000) {
			scene = 3;
		}

		player3d->Update();
		player3d1->Update();
		player3d2->Update();
	}

	if (scene == 2) {
		checker = 0;
		if (input->PushKey(DIK_ESCAPE)) {
			scene = 0;
		}
	}

	if (scene == 3) {
		if (input->PushKey(DIK_ESCAPE)) {
			scene = 0;
		}
	}

}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	// コマンドリストの取得
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//dxCommon->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	if (scene == 0) {
		Sprite::PreDraw(dxCommon->GetCmdList());

		title->Draw();

		Sprite::PostDraw();
	}

	if (scene == 1) {
		if (background == 1) {
			Sprite::PreDraw(dxCommon->GetCmdList());

			last->Draw();

			Sprite::PostDraw();
		}

		if (background == 0) {
			Sprite::PreDraw(dxCommon->GetCmdList());

			spritetest->Draw();

			Sprite::PostDraw();
		}

	}

	if (scene == 2) {
		Sprite::PreDraw(dxCommon->GetCmdList());

		clear->Draw();

		Sprite::PostDraw();
	}

	if (scene == 3) {
		Sprite::PreDraw(dxCommon->GetCmdList());

		gameover->Draw();

		Sprite::PostDraw();
	}

	Sprite::PreDraw(dxCommon->GetCmdList());
	debugText->DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	//オブジェクト前処理
	if (scene == 1) {
		Object3d::PreDraw(dxCommon->GetCmdList());

		//オブジェクトの描画
		for (int i = 0; i < 20; i++) {
			if (enemyF[i] == 0) {
				block3d[i]->Draw();
			}
		}
		if (move == 0) {
			player3d->Draw();
		}
		else if (move == 1) {
			player3d1->Draw();
		}
		else if (move == 2) {
			player3d2->Draw();
		}
		//オブジェクト後処理
		Object3d::PostDraw();
	}
}