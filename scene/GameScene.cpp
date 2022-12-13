
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
#include"DebugScene.h"
#include"ModelManager.h"


using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
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


	playermodel = ObjModel::CreateFromOBJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(playermodel);


	ground = ObjModel::CreateFromOBJ("Field2", true);
	groundObj = Object3d::Create(ground);

	worldmodel = ObjModel::CreateFromOBJ("world", true);
	world = Object3d::Create(worldmodel);

	startModel = ObjModel::CreateFromOBJ("bil", true);
	Start = Object3d::Create(startModel);

	heriM = ObjModel::CreateFromOBJ("heri", true);
	heri = Object3d::Create(heriM);

	haneM = ObjModel::CreateFromOBJ("hane", true);
	hane = Object3d::Create(haneM);

	Goal = Object3d::Create(heriM);

	mid = std::make_unique <middle>();
	mid->Initialize();
	patern = mid->GetPatern();

	playerHp = mid->GetHp();
	oldHp = playerHp;

	postEffect = new PostEffect();
	postEffect->Initialize();

	clearT = 0;
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


	hane->SetRotation({ 0.0f,heriY,0.0f });
	if (move == false) {
		hane->SetPosition(heripos);
		hane->SetScale(heriscl);
	}
	else {
		hane->SetPosition(GoalPos);
		hane->SetScale(GoalScl);
	}

	heri->SetPosition(heripos);
	heri->SetScale(heriscl);
	heri->SetRotation({ 10.0f,180.0f,0.0f });

	Goal->SetPosition(GoalPos);
	Goal->SetScale(GoalScl);
	Goal->SetRotation({ 0.0f,90.0f,0.0f });

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
	//lightGroup->Update();
	for (int i = 0; i < BILLS; i++) {
		bills[i]->Updata({ 1.0f,1.0f,0.7f,0.7f });
		bills1[i]->Updata({ 1.0f,0.5f,0.0f,0.9f });
	}

	sphere->Updata();
	groundObj->Updata();
	world->Updata();
	Start->Updata();
	mid->Updata();
	heri->Updata({ 0.7f,0.7f,0.6f,1.0f });
	Goal->Updata({ 0.7f,0.7f,0.6f,1.0f });
	hane->Updata({ 0.7f,0.7f,0.6f,1.0f });
}

void GameScene::Updata()
{
	heripos.m128_f32[2] += heriX;
	heriY += 15.0f;

	if (heripos.m128_f32[2] >= 20) {
		backObj = false;
		move = true;
	}

	billsposY += 0.5f;
	if (billsposY >= 0) {
		billsposY = 0.0f;
	}

	if (oldHp > playerHp) {
		post = true;
		oldHp = playerHp;
	}
	if (post == true) {
		col.x = 0.7f;
		if (col.x >= 0.5f) {
			post = false;
		}
	}
	if (post == false) {
		col.x -= 0.1f;
		if (col.x <= 0) {
			col.x = 0;
		}
	}


	if (playerHp <= 0) {
		BaseScene* scene_ = new GameOverScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}

	if (patern >= 6) {
		clearT += 0.1f;
		if (clearT >= 25) {
			GoalPos.m128_f32[1] += 0.1f;
			clearT = 25.0f;
		}
	}

	/*if (clearT >= 10) {
		BaseScene* scene_ = new ClearScene(sceneManager_);
		sceneManager_->SetNextScene(scene_);
	}*/

	SetPosSclRot();
	postEffect->Update(col);
	AllUpdata();
}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{

	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	sphere->Draw();
	groundObj->Draw();
	world->Draw();

	Start->Draw();
	Goal->Draw();
	hane->Draw();
	if (backObj == true) {
		heri->Draw();
	}
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
	delete playermodel;
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

