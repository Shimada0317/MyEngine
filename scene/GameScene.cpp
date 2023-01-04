
#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"SceneManager.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"ClearScene.h"
#include"GameOverScene.h"
#include"TitleScene.h"
#include"DebugScene.h"
#include"ModelManager.h"


using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
	postEffect = new PostEffect();
	postEffect->Initialize();
	postEffect->Update(col);

	ModelManager::GetInstance()->Initialize();

	////スプライトの読み込み

	//lightGroup = LightGroup::Create();
	//Object3d::SetLightGroup(lightGroup);
	light = Light::Create();
	Object3d::SetLight(light);

	//モデルの読み込み

	billsModel = ObjModel::CreateFromOBJ("bills");
	for (int i = 0; i < BILLS; i++) {
		bills[i] = Object3d::Create(billsModel);
		bills1[i] = Object3d::Create(billsModel);
	}


	spheremodel = ObjModel::CreateFromOBJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(spheremodel);


	ground = ObjModel::CreateFromOBJ("Field2", true);
	groundObj = Object3d::Create(ground);

	worldmodel = ObjModel::CreateFromOBJ("world", true);
	world = Object3d::Create(worldmodel);

	startModel = ObjModel::CreateFromOBJ("bil", true);
	Start = Object3d::Create(startModel);



	mid = std::make_unique <middle>();
	mid->Initialize();
	patern = mid->GetPatern();

	playerHp = mid->GetHp();
	oldHp = playerHp;

	

	Sprite::LoadTexture(35, L"Resources/Mision.png");
	Comp = Sprite::SpriteCreate(35, { 0.0f,0.0f });

	Sprite::LoadTexture(36, L"Resources/CONTINUE.png");
	Conteniu = Sprite::SpriteCreate(36, { 0.0f,0.0f });

	PostEffectDraw(dxComon);
}

void GameScene::SetPosSclRot()
{
	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition(pos);
	sphere->SetScale({ 4.0f,4.0f,4.0f });

	groundObj->SetPosition(ground_pos);
	groundObj->SetScale({ ground_scl });

	world->SetPosition(worldPos);
	world->SetScale(worldScl);

	patern = mid->GetPatern();
	playerHp = mid->GetHp();

	Start->SetPosition(start_pos);
	Start->SetScale(start_scl);
	Start->SetRotation({ 0.0f,180.0f,0.0f });


	for (int i = 0; i < BILLS; i++) {

		bills[i]->SetScale(billsScl);
		bills1[i]->SetScale(billsScl);
		if (i % 2 == 0) {
			billsPos = { 100.0f, 0,-300.0f + (100 * i / 2) };
			billsPos1 = { 200.0f,0,-300.0f + (100 * i / 2) };
			billsRot = { 0.0f,90.0f,0.0f };
		}
		else if (i % 2 == 1) {
			billsPos = { -100.0f,0,-300.0f + (100 * i / 2) };
			billsPos1 = { -200.0f, 0,-300.0f + (100 * i / 2) };
			billsRot = { 0.0f,270.0f,0.0f };
		}
		bills[i]->SetRotation(billsRot);
		bills[i]->SetPosition(billsPos);
		bills1[i]->SetRotation(billsRot);
		bills1[i]->SetPosition(billsPos1);
	}
};

void GameScene::AllUpdata()
{
	if (GameStart == true) {

		mid->Updata();
	}

		//lightGroup->Update();
		for (int i = 0; i < BILLS; i++) {
			bills[i]->Updata({ 0.2f,0.2f,0.2f,0.7f });
			bills1[i]->Updata({ 1.0f,0.5f,0.0f,0.9f });
		}

		sphere->Updata();
		groundObj->Updata({ 0.7f,0.7f,0.7f,1.0f });
		world->Updata({ 0.1f,0.1f,0.1f,1.0f });
		Start->Updata();
}

void GameScene::Updata()
{
	
	if (GameStart == false) {
		col.x += 0.05f;
		col.y += 0.05f;
		col.z += 0.05f;
		if (col.x >= 0.0f) {
			col.x = 0.0f;
			col.y = 0.0f;
			col.z = 0.0f;
			GameStart = true;
		}
	}
	if (GameStart == true) {
		if (playerHp > 0) {
			//ダメージを食らったたとき
			if (oldHp > playerHp) {
				post = true;
				oldHp = playerHp;
			}
			//画面を赤くするフラグが立った時
			if (post == true) {
				col.x = 0.7f;
				if (col.x >= 0.5f) {
					post = false;
				}
			}
			//画面を赤くするフラグが立っていない時
			if (post == false) {
				col.x -= 0.1f;
				if (col.x <= 0) {
					col.x = 0;
				}
			}
		}
		//体力が0になったら
		else if (playerHp <= 0) {
			gamestop = true;
			col.x += 0.01f;
			if (col.x >= 2.0f) {
				deth = true;
			}
		}
	}
	if (deth == true) {
		col.x = 0;
		if (Input::GetInstance()->PushClick(1)) {
			BaseScene* scene_ = new TitleScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
		else if (Input::GetInstance()->PushClick(0)) {
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}

	XMVECTOR GoalPos = mid->GetGoalPos();
	//ゴールに着いたとき
	if (GoalPos.m128_f32[1] >= 100) {
		complete = true;
		gamestop = true;
	}
	//ゴールについていないとき更新を続ける
	if (gamestop == false) {
		SetPosSclRot();
		AllUpdata();
	}
	//ゴールに着いたらクリア画面を表示
	if (complete == true) {
		if (Input::GetInstance()->PushClick(0)) {
			BaseScene* scene_ = new TitleScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
	postEffect->Update(col);
}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{

	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	sphere->Draw();
	groundObj->Draw();
	world->Draw();

	Start->Draw();

	for (int i = 0; i < BILLS; i++) {
		bills[i]->Draw();
		bills1[i]->Draw();
	}

	////human3d->Draw();
	////オブジェクト後処理
	Object3d::PostDraw();
	mid->Draw(dxCommon);

}

void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	mid->SpriteDraw();
	if (complete == true) {
		Comp->Draw();
	}
	if (deth == true) {
		Conteniu->Draw();
	}
	Sprite::PostDraw();
}

void GameScene::ImgDraw()
{
	mid->ImGuiDraw();
}


void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{

	postEffect->PreDrawScene(dxCommon->GetCmdList());
	ObjDraw(dxCommon);
	SpriteDraw(dxCommon);
	postEffect->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());
	ImgDraw();
	//描画後処理
	dxCommon->PostDraw();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	PostEffectDraw(dxCommon);
}

void GameScene::Finalize()
{
	delete ground;
	delete spheremodel;
	delete worldmodel;
	delete postEffect;
	delete billsModel;
	delete heriM;
	delete startModel;

	world.reset();
	Start.reset();
	groundObj.reset();
	heri.reset();
	Goal.reset();

	for (int i = 0; i < BILLS; i++) {
		bills[i].reset();
		bills1[i].reset();
	}
	mid.reset();
}

