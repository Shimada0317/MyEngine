
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
//コンストラクタ
GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//初期化処理
void GameScene::Initialize(DirectXCommon* dxComon)
{
	//ポストエフェクトの生成
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
	
	Act = std::make_unique <Actor>();
	Act->Initialize();
	Patern = Act->GetPatern();
	PlayerHp = Act->GetHp();
	OldHp = PlayerHp;

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

//ステータスセット
void GameScene::StatusSet()
{
	//天球のステータスセット
	Sphere->SetRotation(Sphere_Rot);
	Sphere->SetPosition(Sphere_Pos);
	Sphere->SetScale(Sphere_Scl);

	//地面のステータスセット
	World->SetPosition(World_Pos);
	World->SetScale(World_Scl);

	//アクタークラスからゲット
	Patern = Act->GetPatern();
	PlayerHp = Act->GetHp();

	//スタート地点のステータスセット
	Start->SetPosition(Start_Pos);
	Start->SetScale(Start_Scl);
	Start->SetRotation(Start_Rot);

	//左右のビルのステータスセット
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

	//フィールドの建物のステータスセット
	for (int i = 0; i < 5; i++) {
		FieldBills[i]->SetPosition(FieldBill_pos[i]);
		FieldBills[i]->SetRotation(FieldBill_Rot[i]);
		FieldBills[i]->SetScale(FieldBill_Scl);
	}
};

//オブジェクトなどの更新処理
void GameScene::AllUpdata()
{
	//ゲーム開始時にアクターを更新処理
	if (GameStart == true) {
		Act->Update();
	}
	//左右のビルの更新処理
	for (int i = 0; i < BILLS; i++) {
		Bills[i]->Update({ 0.4f,0.4f,0.5f,0.9f });
		Bills1[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}
	//フィールドのビルの更新処理
	for (int i = 0; i < 5; i++) {
		FieldBills[i]->Update({ 0.2f,0.2f,0.3f,1.0f });
	}
	//天球の更新処理
	Sphere->Update();
	//地面の更新処理
	World->Update({ 0.7f,0.7f,0.7f,1.0f });
	//スタート地点の更新処理
	Start->Update();
}

//ゲームシーンの更新処理
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

	XMVECTOR GoalPos = Act->GetGoalPos();
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

//オブジェクトの描画処理
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
	////オブジェクト後処理
	Object3d::PostDraw();
	Act->Draw(dxCommon);

}

//スプライトの描画処理
void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	Act->SpriteDraw();
	if (ClearFlag == true) {
		Clear->Draw();
	}
	if (DethFlag == true) {
		Conteniu->Draw();
	}
	Sprite::PostDraw();
}

//ImgUiの描画処理
void GameScene::ImgDraw()
{
	Act->ImGuiDraw();
}

//ポストエフェクトの描画処理
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

//描画処理
void GameScene::Draw(DirectXCommon* dxCommon)
{
	PostEffectDraw(dxCommon);
}

//終了処理
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
	Act.reset();
}

