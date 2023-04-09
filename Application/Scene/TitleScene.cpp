#include "TitleScene.h"
#include"SceneManager.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"GameScene.h"
#include"Mouse.h"

const float SubColor = 0.01f;
const float CameraMoveValueXandY = 0.4f;
const float CameraMoveValueZ = 0.1f;
const float CameraEyeMoveValue = 0.01f;
const XMFLOAT4 RegularColor = { 1.f,1.f,1.f,1.f };
const XMFLOAT2 DescriptionScreenPosition = { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f };

//コンストラクタ
TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//初期化処理
void TitleScene::Initialize(DirectXCommon* dxComon)
{
	TitleCamera = make_unique<Camera>(WinApp::window_width, WinApp::window_height); 
	Object3d::SetCamera(TitleCamera.get());


	lightGroupe = make_unique<LightGroup>();
	lightGroupe = LightGroup::Create();

	Object3d::SetLightGroup(lightGroupe.get());

	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/start.png");
	Sprite::LoadTexture(2, L"REsources/mark2.png");
	Sprite::LoadTexture(3, L"Resources/Click.png");
	Sprite::LoadTexture(4, L"Resources/Click2.png");
	Sprite::LoadTexture(5, L"Resources/SIGNAL.png");
	Sprite::LoadTexture(6, L"Resources/SIGNAL2.png");
	Sprite::LoadTexture(7, L"Resources/setumei2.png");
	Sprite::LoadTexture(8, L"Resources/setumei.png");
	Sprite::LoadTexture(9, L"Resources/setumei3.png");
	Sprite::LoadTexture(10, L"Resources/arrowRight.png");
	Sprite::LoadTexture(11, L"Resources/arrowLeft.png");



	//スプライトの生成
	Title.reset(Sprite::SpriteCreate(1, { 1.0f,1.0f }));
	Cursor.reset(Sprite::SpriteCreate(2, ReticlePos, SpriteCol, Anchorpoint));
	ClickBefore.reset(Sprite::SpriteCreate(3, ClickPos, RegularColor, Anchorpoint));
	ClickAfter.reset(Sprite::SpriteCreate(4, ClickPos, RegularColor, Anchorpoint));
	SignalBefore.reset(Sprite::SpriteCreate(5, ClickPos, RegularColor, Anchorpoint));
	SignalAfter.reset(Sprite::SpriteCreate(6, ClickPos, RegularColor, Anchorpoint));
	DescriptionOperation.reset(Sprite::SpriteCreate(7, DescriptionScreenPosition, SpriteCol, Anchorpoint));
	EnemyOverview.reset(Sprite::SpriteCreate(8, DescriptionScreenPosition, SpriteCol, Anchorpoint));
	GameStartPreparation.reset(Sprite::SpriteCreate(9, DescriptionScreenPosition, SpriteCol, Anchorpoint));
	ArrowRight.reset(Sprite::SpriteCreate(10, ArrowRightPos, ArrowRightColor, Anchorpoint));
	ArrowLeft.reset(Sprite::SpriteCreate(11, ArrowLeftPos, ArrowLeftColor, Anchorpoint));


	//オブジェクトの生成
	Sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(6));
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
		BillsLowAlpha[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
	}
	Start = Object3d::Create(ModelManager::GetInstance()->GetModel(8));
	World = Object3d::Create(ModelManager::GetInstance()->GetModel(9));
	//SEの初期か
	ClickSe = make_unique<Audio>();
	ClickSe->Initialize();
	Bgm = make_unique<Audio>();
	Bgm->Initialize();
	Bgm->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);
	//ポストエフェクトの初期化
	Post = make_unique<PostEffect>();
	Post->Initialize();
	CameraEyeMove = { 0.0f,0.0f,0.0f };
	//スポットライトをアクティブ状態
	lightGroupe->SetSpotLightActive(0, true);
	lightGroupe->SetSpotLightActive(1, true);
	lightGroupe->SetSpotLightActive(2, true);
}

//ステータスセット
void TitleScene::StatusSet()
{
	//カメラの動き
	TitleCamera->MoveEyeVector(CameraEyeMove);
	TitleCamera->MoveVector(CameraMove);
	//天球のステータスのセット
	Sphere->SetRotation(SphereRot);
	Sphere->SetPosition(SpherePos);
	Sphere->SetScale(SphereScl);
	//地面のステータスのセット
	World->SetPosition(WorldPos);
	World->SetScale(WorldScl);
	//左右のビルのステータスのセット
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
	//カメラの移動先のビルのステータスセット
	Start->SetPosition(StartPos);
	Start->SetScale(StartScl);
	Start->SetRotation({ 0.0f,180.0f,0.0f });
	//タイトルスプライトのステータスセット
	Title->SetSize({ TitleSize });
	Title->SetPosition({ TitlePos });
	//マウスカーソルの座標セット
	Cursor->SetPosition({ ReticlePos });

	//1つ目のスポットライトを設定
	lightGroupe->SetSpotLightDir(0, XMVECTOR({ SpotLightDir.x, SpotLightDir.y, SpotLightDir.z }));
	lightGroupe->SetSpotLightPos(0, SpotLightPos);
	lightGroupe->SetSpotLightColor(0, SpotLightColor);
	lightGroupe->SetSpotLightAtten(0, SpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(0, SpotLightFactorAngle);

	//2つ目のスポットライトを設定
	lightGroupe->SetSpotLightDir(1, XMVECTOR({ SpotLightDir2.x, SpotLightDir2.y, SpotLightDir2.z }));
	lightGroupe->SetSpotLightPos(1, SpotLightPos2);
	lightGroupe->SetSpotLightColor(1, SpotLightColor2);
	lightGroupe->SetSpotLightAtten(1, SpotLightAtten2);
	lightGroupe->SetSpotLightFactorAngle(1, SpotLightFactorAngle2);

	//1つ目のスポットライトを設定
	lightGroupe->SetSpotLightDir(2, XMVECTOR({ SpotLightDir.x, SpotLightDir.y, SpotLightDir.z }));
	lightGroupe->SetSpotLightPos(2, SpotLightPos);
	lightGroupe->SetSpotLightColor(2, SpotLightColor);
	lightGroupe->SetSpotLightAtten(2, SpotLightAtten);
	lightGroupe->SetSpotLightFactorAngle(2, SpotLightFactorAngle);


	ArrowLeft->SetSize(ArrowSize);
	ArrowRight->SetSize(ArrowSize);
	ArrowLeft->SetColor(ArrowLeftColor);
	ArrowRight->SetColor(ArrowRightColor);

	ClickAfter->SetSize(ClickSize);
	ClickBefore->SetSize(ClickSize);
	SignalAfter->SetSize(ClickSize);
	SignalBefore->SetSize(ClickSize);

}

//全ての更新処理をまとめる
void TitleScene::AllUpdate()
{
	//左右のビルの更新処理
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->Update({ 0.8f,0.6f,0.3f,1.0f });
		BillsLowAlpha[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}
	//ポストエフェクトの更新処理
	Post->Update(PostEfectColor);
	//天球の更新処理
	Sphere->Update({ 1,1,1,1 },true);
	//地面の更新処理
	World->Update();
	//カメラの移動先のビルの更新処理
	Start->Update({ 0.4f,0.4f,0.4f,1.f });
	//ライトグループ更新
	lightGroupe->Update();
}

//更新処理
void TitleScene::Update()
{
	if (EasingChangeFlag == false) {
		EasingTimer += 0.05f;
		if (EasingTimer >= 1) {
			EasingChangeFlag = true;
		}
	}
	else {
		EasingTimer -= 0.05f;
		if (EasingTimer <= -1) {
			EasingChangeFlag = false;
		}
	}
	ArrowSize.x = Action::GetInstance()->EasingOut(EasingTimer, 5) + 32;
	ArrowSize.y = Action::GetInstance()->EasingOut(EasingTimer, 5) + 32;
	ClickSize.x= Action::GetInstance()->EasingOut(EasingTimer, 5) + 550;
	ClickSize.y= Action::GetInstance()->EasingOut(EasingTimer, 5) + 60;
	Mouse::GetInstance()->MouseMoveSprite(ReticlePos);
	//カメラのムーブ関数
	CameraDirection();
	//カーソルがスプライトの範囲内であるか
	CheckCursorIn(ReticlePos, ClickPos, 250.f, 50.f, SignalCurorInFlag);
	//最初のクリック
	if (SignalCurorInFlag == true && TitleDisplay_F == true) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			TitleSprite_F = false;
			CameraEyeMove_F = true;
			TitleDisplay_F = false;
		}
	}

	DescriptionPageProces();

	FadeOutAndSceneChange();

	StatusSet();

	AllUpdate();
	//カメラの再計算
	TitleCamera->RecalculationMatrix();
}

//カメラの移動
void TitleScene::CameraDirection()
{
	CameraMove = { 0.0f,0.0f,0.0f };
	if (CameraEyeMove_F == true && CameraChange_F == false) {
		if (CameraEyeMove.m128_f32[0] >= -1.1) {
			CameraEyeMove.m128_f32[0] -= CameraEyeMoveValue;
			CameraMove.m128_f32[0] += CameraMoveValueXandY;
			CameraMove.m128_f32[1] -= CameraMoveValueXandY;
			CameraMove.m128_f32[2] -= CameraMoveValueZ;
		}
		else {
			CameraChange_F = true;
		}
	}

	if (CameraChange_F == true) {
		CameraEyeMove = { 0.0f,0.0f,0.0f };
	}
}

//カーソルが範囲内に入っているか
void TitleScene::CheckCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY, bool& CheckFlag)
{
	if ((check_Pos.x - radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) &&
		(check_Pos.y - radY <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		CheckFlag = true;
	}
	else {
		CheckFlag = false;
	}
}

//矢印のスプライトの範囲
bool TitleScene::NextorBack(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY)
{
	if ((check_Pos.x - radX <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) &&
		(check_Pos.y - radY <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		return true;
	}

	return false;
}


void TitleScene::DescriptionPageProces()
{
	//カメラが移動した後の画面
	if (DescriptionPage < 2 && TitleDisplay_F == false && CameraChange_F == true) {
		if (NextorBack(ReticlePos, ArrowRightPos, 16, 16)) {
			ArrowRightColor = { 1.f,0.f,0.f,1.f };
			RightTrueIn_F = true;
			//矢印を押された時
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", Volume);
				DescriptionPage += 1;
			}
		}
		else {
			ArrowRightColor = { 1.f,1.f,1.f,1.f };
			RightTrueIn_F = false;
		}
	}

	//ページが0以上であれば
	if (DescriptionPage > 0) {
		if (NextorBack(ReticlePos, ArrowLeftPos, 16, 16)) {
			ArrowLeftColor = { 1.f,0.f,0.f,1.f };
			LeftTrueIn_F = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", Volume);
				DescriptionPage -= 1;
			}
		}
		else {
			ArrowLeftColor = { 1.f,1.f,1.f,1.f };
			LeftTrueIn_F = false;
		}
	}

	//救援ヘリを呼ぶとき
	if (CameraChange_F == true && SignalCurorInFlag == true && DescriptionPage == 2) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (Click_F == true) {
				ClickSe->LoadFile("Resources/Sound/SE/MorseCode.wav", Volume);
				Click_F = false;
			}
			FadeOut_F = true;
		}
	}
}

//フェードアウト後にゲームシーンへチェンジ
void TitleScene::FadeOutAndSceneChange()
{
	//救援ヘリを読んだ後
	if (FadeOut_F == true) {
		PostEfectColor.x -= SubColor;
		PostEfectColor.y -= SubColor;
		PostEfectColor.z -= SubColor;
		PostEfectColor.w -= SubColor;
		if (PostEfectColor.w <= 0) {
			//シーン切り替え
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}
}

//描画処理
void TitleScene::Draw(DirectXCommon* dxCommon)
{
	Post->PreDrawScene(dxCommon->GetCmdList());

	Object3d::PreDraw(dxCommon->GetCmdList());
	World->Draw();
	Sphere->Draw();
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i]->Draw();
		BillsLowAlpha[i]->Draw();
	}
	Start->Draw();
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	if (TitleSprite_F == true) {
		Title->Draw();
	}
	if (CameraEyeMove_F == false) {
		if (SignalCurorInFlag == false) {
			ClickBefore->Draw();
		}
		else if (SignalCurorInFlag == true) {
			ClickAfter->Draw();
		}
	}
	else if (CameraChange_F == true) {
		if (DescriptionPage < 2) {
			ArrowRight->Draw();
		}
		if (DescriptionPage > 0) {

			ArrowLeft->Draw();


		}
		if (DescriptionPage == 0) {
			DescriptionOperation->Draw();
		}
		else if (DescriptionPage == 1) {
			EnemyOverview->Draw();
		}
		else if (DescriptionPage == 2) {
			GameStartPreparation->Draw();
		}
		if (DescriptionPage == 2) {
			if (SignalCurorInFlag == false) {
				SignalBefore->Draw();
			}
			else if (SignalCurorInFlag == true) {
				SignalAfter->Draw();
			}
		}
	}
	Cursor->Draw();
	Sprite::PostDraw();
	Post->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	Post->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}

//終了処理
void TitleScene::Finalize()
{

	Title.reset();
	ArrowLeft.reset();
	ArrowRight.reset();

	EnemyOverview.reset();
	DescriptionOperation.reset();
	GameStartPreparation.reset();
	SignalAfter.reset();
	SignalBefore.reset();
	Cursor.reset();
	for (int i = 0; i < BILLS; i++) {
		BillsHighAlpha[i].reset();
		BillsLowAlpha[i].reset();
	}
	Sphere.reset();
	World.reset();
	Start.reset();
}