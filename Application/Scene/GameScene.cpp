#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"HelperMath.h"
#include"SceneManager.h"
#include"ModelManager.h"
#include"Mouse.h"

const float AddPosetEfectColor = 0.05f;
const XMFLOAT4 BillColor = { 0.8f,0.6f,0.3f,1.0f };

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
	lightGroupe = make_unique<LightGroup>();
	lightGroupe = LightGroup::Create();

	Object3d::SetLight(light.get());
	Object3d::SetLightGroup(lightGroupe.get());

	//	スプライトの読み込み
	Sprite::LoadTexture(30, L"Resources/DamageEfect.png");
	Sprite::LoadTexture(31, L"Resources/Mision.png");
	Sprite::LoadTexture(32, L"Resources/CONTINUE.png");
	Sprite::LoadTexture(33, L"Resources/Shot.png");
	Sprite::LoadTexture(34, L"Resources/mark2.png");
	Sprite::LoadTexture(35, L"Resources/Yes.png");
	Sprite::LoadTexture(36, L"Resources/No.png");

	//スプライトの生成
	DamageEfectSp.reset(Sprite::SpriteCreate(30, { 0.0f, 0.0f }, DamageEfectColor));
	Clear.reset(Sprite::SpriteCreate(31, { 0.0f,0.0f }));
	Conteniu.reset(Sprite::SpriteCreate(32, { 0.0f,0.0f }));
	Shot.reset(Sprite::SpriteCreate(33, { 0.f,WinApp::window_height - 150 }));
	ReticleForGameOver.reset(Sprite::SpriteCreate(34, ReticlePosition, ReticleColor, SpriteAnchorPoint));
	Yes.reset(Sprite::SpriteCreate(35, YesPosition,YesColor,SpriteAnchorPoint));
	No.reset(Sprite::SpriteCreate(36, NoPosition,NoColor,SpriteAnchorPoint));


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


	for (int i = 0; i < 3; i++) {
		SearchLightDir[i] = { 0,-10,0 };
		SearchLightColor[i] = { 1.f,1.f,1.f };
	}
	SearchLightPos[0] = { 0, 20, 20 };
	SearchLightPos[1] = { 20, 10, 45 };
	SearchLightPos[2] = { 54,10,43 };
	for (int i = 0; i < 2; i++) {
		LightPositionChangeX[i] = false;
		LightPositionChangeZ[i] = false;
	}

	lightGroupe->SetSpotLightActive(0, true);
	lightGroupe->SetSpotLightActive(1, true);
	lightGroupe->SetSpotLightActive(2, true);
	lightGroupe->SetSpotLightActive(3, true);
	lightGroupe->SetSpotLightActive(4, true);
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

	for (int i = 2; i < 5; i++) {
		lightGroupe->SetSpotLightDir(i, XMVECTOR({ SearchLightDir[i - 2].x, SearchLightDir[i - 2].y, SearchLightDir[i - 2].z }));
		lightGroupe->SetSpotLightPos(i, SearchLightPos[i - 2]);
		lightGroupe->SetSpotLightColor(i, SearchLightColor[i - 2]);
		lightGroupe->SetSpotLightAtten(i, SearchLightAtten);
		lightGroupe->SetSpotLightFactorAngle(i, SearchLightFactorAngle);
	}

};

//オブジェクトなどの更新処理
void GameScene::AllUpdata()
{
	Action::GetInstance()->DebugMove(SearchLightPos[0]);
	//ゲーム開始時にアクターを更新処理
	if (GameStartFlag == true) {
		Act->Update();
	}
	//左右のビルの更新処理
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->Update(BillColor);
		BillsLowAlpha[i]->Update(BillColor);
	}
	//フィールドのビルの更新処理
	for (int i = 0; i < 5; i++) {
		FieldBills[i]->Update(BillColor);
	}
	//天球の更新処理
	Sphere->Update({ 1,1,1,1 },true);
	//地面の更新処理
	World->Update({ 0.7f,0.7f,0.7f,1.0f });
	//スタート地点の更新処理
	Start->Update(BillColor);
}

//ゲームシーンの更新処理
void GameScene::Update()
{

	SpotLightMove();

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
	else if(StopUpdateFlag==true&&ClearFlag==false)
	{
		GameOverProcess();
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
	Shot->Draw();
	Act->SpriteDraw();
	if (DamageHitFlag == true) {
		DamageEfectSp->Draw();
	}
	if (ClearFlag == true) {
		Clear->Draw();
	}
	if (DethFlag == true) {
		Conteniu->Draw();
		//if (NoCursorInFlag == false) {
			No->Draw();
		//}
		/*else {
			NoAfter->Draw();
		}*/
		//if (YesCursorInFlag == false) {
			Yes->Draw();
		//}
		/*else {
			YesAfter->Draw();
		}*/
		ReticleForGameOver->Draw();
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
	/*ImGui::SliderFloat("LightPosX", &SearchLightPos[0].x, -100.0f, 100.0f);
	ImGui::SliderFloat("LightPosY", &SearchLightPos[0].y, -100.0f, 100.0f);
	ImGui::SliderFloat("LightPosZ", &SearchLightPos[0].z, -100.0f, 100.0f);*/

	ImGui::SliderFloat("LightDirX", &SearchLightDir[0].x, -100.0f, 100.0f);
	ImGui::SliderFloat("LightDirY", &SearchLightDir[0].y, -100.0f, 100.0f);
	ImGui::SliderFloat("LightDirZ", &SearchLightDir[0].z, -100.0f, 100.0f);
	ImGui::SliderFloat("Red", &FieldSpotLightColor.x, -60.0f, 60.0f);
	ImGui::SliderFloat("Green", &FieldSpotLightColor.y, -60.0f, 60.0f);
	ImGui::SliderFloat("Blue", &FieldSpotLightColor.z, -60.0f, 60.0f);

	ImGui::SliderFloat("value", &value, 0.0f, 60.0f);
	ImGui::SliderFloat("time", &time, -0.0f, 1.0f);
	ImGui::SliderInt("Patern", &Patern, 0, 1);
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
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
	//描画後処理
	//Act->ImGuiDraw();
	//ImgDraw();
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
	Shot.reset();
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

void GameScene::SpotLightMove()
{
	if (Easing == false) {
		EasingWaitTimeR += 0.1f;
		if (EasingWaitTimeR >= 1) {
			Easing = true;
			EasingWaitTimeR = 0.f;
		}
	}

	if (Easing == true) {
		if (EasingChange == false) {
			duration = 1;
			if (duration > time) {
				time += 0.01f;
			}
		}
		else {
			duration = -1;
			if (duration < time) {
				time -= 0.01f;
			}
		}
	}
	if (ChangeTimerFlag == false) {
		LightAddPosChangeTimer += 0.01f;
	}
	else {
		LightAddPosChangeTimer -= 0.01f;
	}
	if (LightAddPosChangeTimer >= 1) {
		SearchLightAddPos = HelperMath::GetInstance()->XMFLOAT3ChangeValue(SearchLightAddPos);
		ChangeTimerFlag = true;
	}
	else if (LightAddPosChangeTimer <= -1) {
		SearchLightAddPos = HelperMath::GetInstance()->XMFLOAT3ChangeValue(SearchLightAddPos);
		ChangeTimerFlag = false;
	}

	if (LightDirEasingChane == false) {
		LightDirEasingTime += 0.05f;
	}
	else {
		LightDirEasingTime -= 0.05f;
	}

	SearchLightDir[0].x = Action::GetInstance()->EasingOut(LightDirEasingTime, 5 - 0);
	SearchLightDir[1].z = Action::GetInstance()->EasingOut(LightDirEasingTime, 5 - 0);
	SearchLightDir[2].x = Action::GetInstance()->EasingOut(LightDirEasingTime, 5 - 0);

	SearchLightDir[0].z = Action::GetInstance()->EasingOut(time, EndPointZ - StartPointZ);
	SearchLightDir[1].x = Action::GetInstance()->EasingOut(time, EndPointX - StartPointX);
	SearchLightDir[2].z = Action::GetInstance()->EasingOut(time, EbdPointZ2 - StartPointZ2);



	bool MoveFlag = Act->GetMove();

	if (Patern == 8 && MoveFlag == true) {
		if (ColorTime >= 0) {
			ColorTime -= 0.01f;
		}
		if (ColorTimeRed <= 1) {
			ColorTimeRed += 0.01f;
		}

		FieldSpotLightColor.x = Action::GetInstance()->EasingOut(ColorTimeRed, EndColorRed - StartColorRed) + 0.9f;
		FieldSpotLightColor.y = Action::GetInstance()->EasingOut(ColorTime, EndColor - StartColor);
	}

	if (SpotLightPositionChange == false) {
		if (time >= 1.f) {
			SpotLightPositionChange = true;
			Easing = false;
			EasingChange = true;
		}
	}
	else {
		if (time <= -1.f) {
			SpotLightPositionChange = false;
			Easing = false;
			EasingChange = false;
		}
	}
}

void GameScene::GameOverProcess()
{
	Mouse::GetInstance()->MouseMoveSprite(ReticlePosition);
	ReticleForGameOver->SetPosition(ReticlePosition);
	if (DethFlag == true) {
		PostCol.x = 0;
		CheckcCursorIn(ReticlePosition, YesPosition, 100, 50, YesCursorInFlag);
		CheckcCursorIn(ReticlePosition, NoPosition, 100, 50, NoCursorInFlag);

		if (YesCursorInFlag == true) {
			YesColor = { 1.f,0.f,0.f,1.f };
			if (Mouse::GetInstance()->PushClick(0)) {
				BaseScene* scene_ = new GameScene(sceneManager_);
				sceneManager_->SetNextScene(scene_);
			}
		}
		else {
			YesColor = { 1.f,1.f,1.f,1.f };
		}

		if (NoCursorInFlag == true) {
			NoColor = { 1.f,0.f,0.f,1.f };
			if (Mouse::GetInstance()->PushClick(0)) {
				BaseScene* scene_ = new TitleScene(sceneManager_);
				sceneManager_->SetNextScene(scene_);
			}
		}
		else {
			NoColor = { 1.f,1.f,1.f,1.f };
		}
		Yes->SetColor(YesColor);
		No->SetColor(NoColor);
	}
}

void GameScene::CheckcCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY, bool& CheckFlag)
{
	if ((check_Pos.x-radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x)
		&& (check_Pos.y-radY<= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		CheckFlag = true;
	}
	else {
		CheckFlag = false;
	}
}

