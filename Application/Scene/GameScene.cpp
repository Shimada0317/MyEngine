
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
	Sprite::LoadTexture(20, L"Resources/DamageEfect.png");
	Sprite::LoadTexture(21, L"Resources/Mision.png");
	Sprite::LoadTexture(22, L"Resources/CONTINUE.png");
	//スプライトの生成
	Clear.reset(Sprite::SpriteCreate(21, { 0.0f,0.0f }));
	Conteniu.reset(Sprite::SpriteCreate(22, { 0.0f,0.0f }));
	DamageEfectSp.reset(Sprite::SpriteCreate(20, { 0.0f, 0.0f }, DamageEfectColor));

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
	lightGroupe->SetSpotLightActive(0, true);
	lightGroupe->SetSpotLightActive(1,true);
	lightGroupe->SetSpotLightActive(2, true);
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

	lightGroupe->SetSpotLightDir(0, XMVECTOR({ FieldSpotLightDir.x, FieldSpotLightDir.y, FieldSpotLightDir.z }));
	lightGroupe->SetSpotLightPos(0, FieldSpotLightPos);
	lightGroupe->SetSpotLightColor(0, FieldSpotLightColor);
	lightGroupe->SetSpotLightAtten(0, FieldSpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(0, FieldSpotLightFactorAngle);

	lightGroupe->SetSpotLightDir(1, XMVECTOR({ PlayerSpotLightDir.x, PlayerSpotLightDir.y, PlayerSpotLightDir.z }));
	lightGroupe->SetSpotLightPos(1, PlayerSpotLightPos);
	lightGroupe->SetSpotLightColor(1, PlayerSpotLightColor);
	lightGroupe->SetSpotLightAtten(1, PlayerSpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(1, PlayerSpotLightFactorAngle);

	lightGroupe->SetSpotLightDir(2, XMVECTOR({ SpotLightDir3.x, SpotLightDir3.y, SpotLightDir3.z }));
	lightGroupe->SetSpotLightPos(2, SpotLightPos3);
	lightGroupe->SetSpotLightColor(2, SpotLightColor3);
	lightGroupe->SetSpotLightAtten(2, SpotLightAtten3);
	lightGroupe->SetSpotLightFactorAngle(2, SpotLightFactorAngle3);
};

//オブジェクトなどの更新処理
void GameScene::AllUpdata()
{
	int Wave = Act->GetPatern();

	XMVECTOR velocity = Act->GetVelocity();
	PlayerSpotLightPos.x += velocity.m128_f32[0];
	PlayerSpotLightPos.z += velocity.m128_f32[2];
	if (Wave <= 4 && Wave < 5) {
		if (SpotLightPositionChange == false) {
			Action::GetInstance()->EaseOut(SpotLightPos3.z, 61);
			if (SpotLightPos3.z >= 60) {
				SpotLightPositionChange = true;
			}
		}
		else {
			Action::GetInstance()->EaseOut(SpotLightPos3.z, -11);
			if (SpotLightPos3.z <= -10) {
				SpotLightPositionChange = false;
			}
		}
	}

	
	if (Wave > 4 && Wave < 8) {
		SpotLightPos3.z = 45;
		if (SpotLightPositionChange == false) {
			Action::GetInstance()->EaseOut(SpotLightPos3.x, 61);
			if (SpotLightPos3.x >= 60) {
				SpotLightPositionChange = true;
			}
		}
		else {
			Action::GetInstance()->EaseOut(SpotLightPos3.x, -11);
			if (SpotLightPos3.x <= -10) {
				SpotLightPositionChange = false;
			}
		}
	}

	if (Wave == 8 || Wave == 9) {
		if (SpotLightPos3.x <= 60) {
			Action::GetInstance()->EaseOut(SpotLightPos3.x, 61);
		}

		if (SpotLightPositionChange == false) {
			Action::GetInstance()->EaseOut(SpotLightPos3.z, 111);
			if (SpotLightPos3.z >= 110) {
				SpotLightPositionChange = true;
			}
		}
		else {
			Action::GetInstance()->EaseOut(SpotLightPos3.z, 49);
			if (SpotLightPos3.z <= 50) {
				SpotLightPositionChange = false;
			}
		}
	}

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
	World->Update({ 0.7f,0.7f,0.7f,1.0f }, true);
	//スタート地点の更新処理
	Start->Update();

}

//ゲームシーンの更新処理
void GameScene::Update()
{





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

	if (ImGui::TreeNode("WorldLight")) {
		ImGui::SliderFloat("PosX", &FieldSpotLightPos.x, -100.0f, 100.0f);
		ImGui::SliderFloat("PosY", &FieldSpotLightPos.y, -100.0f, 1000.0f);
		ImGui::SliderFloat("PosZ", &FieldSpotLightPos.z, -100.0f, 100.0f);

		ImGui::SliderFloat("DirX", &FieldSpotLightDir.x, -100.0f, 100.0f);
		ImGui::SliderFloat("DirY", &FieldSpotLightDir.y, -100.0f, 100.0f);
		ImGui::SliderFloat("DirZ", &FieldSpotLightDir.z, -100.0f, 100.0f);

		ImGui::SliderFloat("AttenX", &FieldSpotLightAtten.x, -100.0f, 100.0f);
		ImGui::SliderFloat("AttenY", &FieldSpotLightAtten.y, -100.0f, 100.0f);
		ImGui::SliderFloat("AttenZ", &FieldSpotLightAtten.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("BlueLight")) {
		ImGui::SliderFloat("PosX", &SpotLightPos3.x, -100.0f, 100.0f);
		ImGui::SliderFloat("PosY", &SpotLightPos3.y, -100.0f, 1000.0f);
		ImGui::SliderFloat("PosZ", &SpotLightPos3.z, -100.0f, 100.0f);

		ImGui::SliderFloat("DirX", &SpotLightDir3.x, -100.0f, 100.0f);
		ImGui::SliderFloat("DirY", &SpotLightDir3.y, -100.0f, 100.0f);
		ImGui::SliderFloat("DirZ", &SpotLightDir3.z, -100.0f, 100.0f);

		ImGui::SliderFloat("AttenX", &SpotLightAtten3.x, -100.0f, 100.0f);
		ImGui::SliderFloat("AttenY", &SpotLightAtten3.y, -100.0f, 100.0f);
		ImGui::SliderFloat("AttenZ", &SpotLightAtten3.z, -100.0f, 100.0f);
		ImGui::TreePop();
	}


	Act->ImGuiDraw();

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
	Conteniu.reset();
	Clear.reset();
	delete Bgm;
	delete light;
	delete lightGroupe;
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

