
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
	lightGroupe = LightGroup::Create();

	Object3d::SetLight(light);
	Object3d::SetLightGroup(lightGroupe);

	//	スプライトの読み込み
	Sprite::LoadTexture(19, L"Resources/DamageEfect.png");
	Sprite::LoadTexture(35, L"Resources/Mision.png");
	Sprite::LoadTexture(36, L"Resources/CONTINUE.png");
	//スプライトの生成
	Clear = Sprite::SpriteCreate(35, { 0.0f,0.0f });
	Conteniu = Sprite::SpriteCreate(36, { 0.0f,0.0f });
	DamageEfectSp = Sprite::SpriteCreate(19, { 0.0f, 0.0f }, DamageEfectColor);

	//モデルの読み込み
	Sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(6));
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
		BillsLowAlpha[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));

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

	FieldBillPos[0] = { -35.0f,0.0f,20.0f };
	FieldBillPos[1] = { -7.0f,-1,80 };
	FieldBillPos[2] = { 37.0,0.0f,-10.0f };
	FieldBillPos[3] = { -900.0,0.0f,0.0f };;
	FieldBillPos[4] = { -900.0,0.0f,0.0f };;

	FieldBillRot[0] = { 0.0f,270.0f,0.0f };
	FieldBillRot[1] = { 0.0f,0.0f,0.0f };
	FieldBillRot[2] = { 0.0f,90.0f,0.0f };
	FieldBillRot[3] = { 0.0f,0.0f,0.0f };
	FieldBillRot[4] = { 0.0f,0.0f,0.0f };

	lightGroupe->SetPointLightActive(0, false);
	lightGroupe->SetPointLightActive(1, false);
	lightGroupe->SetPointLightActive(2, false);
	lightGroupe->SetSpotLightActive(0, false);
	lightGroupe->SetSpotLightActive(1, false);
	lightGroupe->SetSpotLightActive(2, false);
}

//ステータスセット
void GameScene::StatusSet()
{
	//天球のステータスセット
	Sphere->SetRotation(SphereRot);
	Sphere->SetPosition(SpherePos);
	Sphere->SetScale(SphereScl);

	//地面のステータスセット
	World->SetPosition(WorldPos);
	World->SetScale(WorldScl);

	//アクタークラスからゲット
	Patern = Act->GetPatern();
	PlayerHp = Act->GetHp();

	//スタート地点のステータスセット
	Start->SetPosition(StartPos);
	Start->SetScale(StartScl);
	Start->SetRotation(StartRot);

	//左右のビルのステータスセット
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->SetScale(BillsScl);
		BillsLowAlpha[i]->SetScale(BillsScl);
		if (i % 2 == 0) {
			BillsHighAlphaPos = { 100.0f, 0,-300.0f + (100 * i / 2) };
			BillsLowAlphaPos = { 200.0f,0,-300.0f + (100 * i / 2) };
			BillsRot = { 0.0f,90.0f,0.0f };
		}
		else if (i % 2 == 1) {
			BillsHighAlphaPos = { -100.0f,0,-300.0f + (100 * i / 2) };
			BillsLowAlphaPos = { -200.0f, 0,-300.0f + (100 * i / 2) };
			BillsRot = { 0.0f,270.0f,0.0f };
		}
		BillsHighAlpha[i]->SetRotation(BillsRot);
		BillsHighAlpha[i]->SetPosition(BillsHighAlphaPos);
		BillsLowAlpha[i]->SetRotation(BillsRot);
		BillsLowAlpha[i]->SetPosition(BillsLowAlphaPos);
	}

	//フィールドの建物のステータスセット
	for (int i = 0; i < 5; i++) {
		FieldBills[i]->SetPosition(FieldBillPos[i]);
		FieldBills[i]->SetRotation(FieldBillRot[i]);
		FieldBills[i]->SetScale(FieldBillScl);
	}

	DamageEfectSp->SetColor(DamageEfectColor);

	lightGroupe->SetSpotLightDir(0, XMVECTOR({ SpotLightDir.x, SpotLightDir.y, SpotLightDir.z }));
	lightGroupe->SetSpotLightPos(0, SpotLightPos);
	lightGroupe->SetSpotLightColor(0, SpotLightColor);
	lightGroupe->SetSpotLightAtten(0, SpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(0, SpotLightFactorAngle);

	lightGroupe->SetSpotLightDir(1, XMVECTOR({ SpotLightDir2.x, SpotLightDir2.y, SpotLightDir2.z }));
	lightGroupe->SetSpotLightPos(1, SpotLightPos2);
	lightGroupe->SetSpotLightColor(1, SpotLightColor2);
	lightGroupe->SetSpotLightAtten(1, SpotLightAtten2);
	lightGroupe->SetSpotLightFactorAngle(1, SpotLightFactorAngle2);

	lightGroupe->SetSpotLightDir(2, XMVECTOR({ SpotLightDir3.x, SpotLightDir3.y, SpotLightDir3.z }));
	lightGroupe->SetSpotLightPos(2, SpotLightPos3);
	lightGroupe->SetSpotLightColor(2, SpotLightColor3);
	lightGroupe->SetSpotLightAtten(2, SpotLightAtten3);
	lightGroupe->SetSpotLightFactorAngle(2, SpotLightFactorAngle3);
};

//オブジェクトなどの更新処理
void GameScene::AllUpdata()
{
	//ゲーム開始時にアクターを更新処理
	if (GameStartFlag == true) {
		Act->Update();
	}
	//左右のビルの更新処理
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->Update({ 0.4f,0.4f,0.5f,1.0f });
		BillsLowAlpha[i]->Update({ 0.2f,0.2f,0.2f,1.0f });
	}
	//フィールドのビルの更新処理
	for (int i = 0; i < 5; i++) {
		FieldBills[i]->Update({ 0.2f,0.2f,0.3f,1.0f });
	}
	//天球の更新処理
	Sphere->Update({ 1,1,1,1 }, true);
	//地面の更新処理
	World->Update({ 0.7f,0.7f,0.7f,1.0f });
	//スタート地点の更新処理
	Start->Update();

}

//ゲームシーンの更新処理
void GameScene::Update()
{

	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		lightGroupe->SetSpotLightActive(0, true);
	}

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		lightGroupe->SetSpotLightActive(1, true);
	}

	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		lightGroupe->SetSpotLightActive(2, true);
	}

	if (Input::GetInstance()->TriggerKey(DIK_3)) {
		lightGroupe->SetSpotLightActive(0, false);
	}

	if (Input::GetInstance()->TriggerKey(DIK_4)) {
		lightGroupe->SetSpotLightActive(1, false);
	}

	if (Input::GetInstance()->TriggerKey(DIK_5)) {
		lightGroupe->SetSpotLightActive(2, false);
	}

	if (GameStartFlag == false) {
		PostCol.x += AddPosetEfectColor;
		PostCol.y += AddPosetEfectColor;
		PostCol.z += AddPosetEfectColor;
		if (PostCol.x >= 0.0f) {
			PostCol.x = 0.0f;
			PostCol.y = 0.0f;
			PostCol.z = 0.0f;
			GameStartFlag = true;
		}
	}
	if (GameStartFlag == true) {
		if (PlayerHp > 0) {
			//ダメージを食らったたとき
			if (OldHp > PlayerHp) {
				PostEffectOnFlag = true;
				DamageHitFlag = true;
				DamageEfectColor.w = 1;
				OldHp = PlayerHp;
			}
			//画面を赤くするフラグが立った時
			if (PostEffectOnFlag == true) {
				PostCol.x = 0.7f;
				if (PostCol.x >= 0.7f) {
					PostEffectOnFlag = false;
				}
			}
			//画面を赤くするフラグが立っていない時
			if (PostEffectOnFlag == false) {
				PostCol.x -= 0.05f;
				if (PostCol.x <= 0) {
					PostCol.x = 0;
				}
			}
		}
		//体力が0になったら
		else if (PlayerHp <= 0) {
			StopUpdateFlag = true;
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

	if (DamageHitFlag == true) {
		DamageEfectColor.w -= 0.02f;
		if (DamageEfectColor.w <= 0) {
			DamageHitFlag = false;
		}
	}

	XMVECTOR GoalPos = Act->GetGoalPos();
	//ゴールに着いたとき
	if (GoalPos.m128_f32[1] >= 100) {
		ClearFlag = true;
		StopUpdateFlag = true;
	}
	//ゴールについていないとき更新を続ける
	if (StopUpdateFlag == false) {
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
	lightGroupe->Update();
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
		BillsHighAlpha[i]->Draw();
		BillsLowAlpha[i]->Draw();
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
	if (DamageHitFlag == true) {
		DamageEfectSp->Draw();
	}
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

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Light");

	ImGui::SliderFloat("PosX", &SpotLightPos2.x, -100.0f, 100.0f);
	ImGui::SliderFloat("PosY", &SpotLightPos2.y, -100.0f, 1000.0f);
	ImGui::SliderFloat("PosZ", &SpotLightPos2.z, -100.0f, 100.0f);

	ImGui::SliderFloat("RotX", &SpotLightDir2.x, -100.0f, 100.0f);
	ImGui::SliderFloat("RotY", &SpotLightDir2.y, -100.0f, 100.0f);
	ImGui::SliderFloat("RotZ", &SpotLightDir2.z, -100.0f, 100.0f);



	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	//Act->ImGuiDraw();
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
	delete postEffect;
	delete Conteniu;
	delete Clear;
	delete Bgm;
	World.reset();
	Start.reset();
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i].reset();
		BillsLowAlpha[i].reset();
	}
	for (int i = 0; i < 5; i++) {
		FieldBills[i].reset();
	}
	Act.reset();
}

