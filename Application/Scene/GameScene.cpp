
#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"SceneManager.h"
#include"ClearScene.h"
#include"TitleScene.h"
#include"DebugScene.h"
#include"ModelManager.h"

const float AddPosetEfectColor = 0.05f;

using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
	postEffect = new PostEffect();
	postEffect->Initialize();
	postEffect->Update(PostCol);

	light = Light::Create();
	Object3d::SetLight(light);

	//	スプライトの読み込み
	Sprite::LoadTexture(35, L"Resources/Mision.png");
	Sprite::LoadTexture(36, L"Resources/CONTINUE.png");
	//スプライトの生成
	Clear = Sprite::SpriteCreate(35, { 0.0f,0.0f });
	Conteniu = Sprite::SpriteCreate(36, { 0.0f,0.0f });

	//モデルの読み込み
	Sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(6));
	for (int i = 0; i < BILLS; i++) {
		Bills[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
		Bills1[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));

	}
	for (int i = 0; i < 5; i++) {
		FieldBills[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
	}
	World = Object3d::Create(ModelManager::GetInstance()->GetModel(9));
	Start = Object3d::Create(ModelManager::GetInstance()->GetModel(8));
	//
	Mid = std::make_unique <Actor>();
	Mid->Initialize();
	Patern = Mid->GetPatern();

	PlayerHp = Mid->GetHp();
	OldHp = PlayerHp;

	//PostEffectDraw(dxComon);

	Bgm = new Audio;
	Bgm->Initialize();
	Bgm->LoopWave("Resources/Sound/BGM/Blinded.wav", 0.3f);

	FieldBill_pos[0] = {-35.0f,0.0f,20.0f};
	FieldBill_pos[1] = { -7.0f,-1,80 };
	FieldBill_pos[2]={37.0,0.0f,-10.0f};
	FieldBill_pos[3] = { -900.0,0.0f,0.0f };;
	FieldBill_pos[4] = { -900.0,0.0f,0.0f };;

	FieldBill_Rot[0]={0.0f,270.0f,0.0f};
	FieldBill_Rot[1]={0.0f,0.0f,0.0f};
	FieldBill_Rot[2] = { 0.0f,90.0f,0.0f };
	FieldBill_Rot[3] = { 0.0f,0.0f,0.0f };
	FieldBill_Rot[4] = { 0.0f,0.0f,0.0f };
}

void GameScene::StatusSet()
{
	Sphere->SetRotation(Sphere_Rot);
	Sphere->SetPosition(Sphere_Pos);
	Sphere->SetScale(Sphere_Scl);

	World->SetPosition(World_Pos);
	World->SetScale(World_Scl);

	Patern = Mid->GetPatern();
	PlayerHp = Mid->GetHp();

	Start->SetPosition(Start_Pos);
	Start->SetScale(Start_Scl);
	Start->SetRotation(Start_Rot);


	for (int i = 0; i < BILLS; i++) {

		Bills[i]->SetScale(Bills_Scl);
		Bills1[i]->SetScale(Bills_Scl);
		if (i % 2 == 0) {
			Bills_Pos = { 100.0f, 0,-300.0f + (100 * i / 2) };
			Bills_Pos1 = { 200.0f,0,-300.0f + (100 * i / 2) };
			Bills_Rot = { 0.0f,90.0f,0.0f };
		}
		else if (i % 2 == 1) {
			Bills_Pos = { -100.0f,0,-300.0f + (100 * i / 2) };
			Bills_Pos1 = { -200.0f, 0,-300.0f + (100 * i / 2) };
			Bills_Rot = { 0.0f,270.0f,0.0f };
		}
		Bills[i]->SetRotation(Bills_Rot);
		Bills[i]->SetPosition(Bills_Pos);
		Bills1[i]->SetRotation(Bills_Rot);
		Bills1[i]->SetPosition(Bills_Pos1);
	}

	for (int i = 0; i < 5; i++) {
		FieldBills[i]->SetPosition(FieldBill_pos[i]);
		FieldBills[i]->SetRotation(FieldBill_Rot[i]);
		FieldBills[i]->SetScale(FieldBill_Scl);
	}
};

void GameScene::AllUpdata()
{
	if (GameStart == true) {

		Mid->Update();
	}

	for (int i = 0; i < BILLS; i++) {
		Bills[i]->Update({ 0.4f,0.4f,0.5f,0.9f });
		Bills1[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}

	for (int i = 0; i < 5; i++) {
		FieldBills[i]->Update({ 0.2f,0.2f,0.3f,1.0f });
	}

	Sphere->Update();

	World->Update({ 0.7f,0.7f,0.7f,1.0f });
	Start->Update();
}

void GameScene::Update()
{

	if (GameStart == false) {
		PostCol.x += AddPosetEfectColor;
		PostCol.y += AddPosetEfectColor;
		PostCol.z += AddPosetEfectColor;
		if (PostCol.x >= 0.0f) {
			PostCol.x = 0.0f;
			PostCol.y = 0.0f;
			PostCol.z = 0.0f;
			GameStart = true;
		}
	}
	if (GameStart == true) {
		if (PlayerHp > 0) {
			//ダメージを食らったたとき
			if (OldHp > PlayerHp) {
				PostEffectOn = true;
				OldHp = PlayerHp;
			}
			//画面を赤くするフラグが立った時
			if (PostEffectOn == true) {
				PostCol.x = 0.7f;
				if (PostCol.x >= 0.7f) {
					PostEffectOn = false;
				}
			}
			//画面を赤くするフラグが立っていない時
			if (PostEffectOn == false) {
				PostCol.x -= 0.05f;
				if (PostCol.x <= 0) {
					PostCol.x = 0;
				}
			}
		}
		//体力が0になったら
		else if (PlayerHp <= 0) {
			StopUpdate = true;
			PostCol.x += 0.01f;
			if (PostCol.x >= 2.0f) {
				DethFlag = true;
			}
		}
	}
	if (DethFlag == true) {
		PostCol.x = 0;
		if (Mouse::GetInstance()->PushClick(1)) {
			BaseScene* scene_ = new TitleScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
		else if (Mouse::GetInstance()->PushClick(0)) {
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}

	XMVECTOR GoalPos = Mid->GetGoalPos();
	//ゴールに着いたとき
	if (GoalPos.m128_f32[1] >= 100) {
		ClearFlag = true;
		StopUpdate = true;
	}
	//ゴールについていないとき更新を続ける
	if (StopUpdate == false) {
		StatusSet();
		AllUpdata();
	}
	//ゴールに着いたらクリア画面を表示
	if (ClearFlag == true) {
		if (Mouse::GetInstance()->PushClick(0)) {
			BaseScene* scene_ = new TitleScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
	postEffect->Update(PostCol);
}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{

	////オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	Sphere->Draw();
	World->Draw();

	Start->Draw();

	for (int i = 0; i < 5; i++) {
		FieldBills[i]->Draw();
	}
	for (int i = 0; i < BILLS; i++) {
		Bills[i]->Draw();
		Bills1[i]->Draw();
	}

	////human3d->Draw();
	////オブジェクト後処理
	Object3d::PostDraw();
	Mid->Draw(dxCommon);

}

void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	Mid->SpriteDraw();
	if (ClearFlag == true) {
		Clear->Draw();
	}
	if (DethFlag == true) {
		Conteniu->Draw();
	}
	Sprite::PostDraw();
}

void GameScene::ImgDraw()
{
	Mid->ImGuiDraw();
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
	delete  SphereModel;
	delete WorldModel;
	delete postEffect;
	delete BillsModel;
	delete StartModel;
	delete Conteniu;
	delete Clear;
	delete Bgm;

	World.reset();
	Start.reset();


	for (int i = 0; i < BILLS; i++) {
		Bills[i].reset();
		Bills1[i].reset();
	}
	for (int i = 0; i < 5; i++) {
		FieldBills[i].reset();
	}
	Mid.reset();
}

