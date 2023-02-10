#include "TitleScene.h"
#include"SceneManager.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"GameScene.h"
#include"Mouse.h"

using namespace DirectX;

const float SubColor = 0.01f;
const float CameraMoveValueXandY = 0.4f;
const float CameraMoveValueZ = 0.1f;
const float CameraEyeMoveValue = 0.01f;

TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

//初期化処理
void TitleScene::Initialize(DirectXCommon* dxComon)
{
	TitleCamera = new Camera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(TitleCamera);

	light = Light::Create();
	Object3d::SetLight(light);

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
	Sprite::LoadTexture(12, L"Resources/arrowRightTrue.png");
	Sprite::LoadTexture(13, L"Resources/arrowLeftTrue.png");

	//スプライトの生成
	Title = Sprite::SpriteCreate(1, { 1.0f,1.0f });
	Cursor = Sprite::SpriteCreate(2, ReticlePos, SpriteCol, Anchorpoint);
	ClickBefore = Sprite::SpriteCreate(3, ClickPos);
	ClickAfter = Sprite::SpriteCreate(4, ClickPos);
	SignalBefore = Sprite::SpriteCreate(5, ClickPos);
	SignalAfter = Sprite::SpriteCreate(6, ClickPos);
	Explanation = Sprite::SpriteCreate(7, {WinApp::window_width/ 2.0f,WinApp::window_height / 2.0f - 72.0f }, SpriteCol, Anchorpoint);
	Explanation2 = Sprite::SpriteCreate(8, { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f }, SpriteCol, Anchorpoint);
	Explanation3 = Sprite::SpriteCreate(9, { WinApp::window_width / 2.0f,WinApp::window_height / 2.0f - 72.0f }, SpriteCol, Anchorpoint);
	ArrowRight = Sprite::SpriteCreate(10, ArrowRightPos);
	ArrowLeft = Sprite::SpriteCreate(11, ArrowLeftPos);
	ArrowRightTrue = Sprite::SpriteCreate(12, ArrowRightPos);
	ArrowLeftTrue = Sprite::SpriteCreate(13, ArrowLeftPos);

	//オブジェクトの生成
	Sphere = Object3d::Create(ModelManager::GetInstance()->GetModel(6));
	for (int i = 0; i < BILLS; i++) {
		Bills[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
		Bills1[i] = Object3d::Create(ModelManager::GetInstance()->GetModel(7));
	}
	Start = Object3d::Create(ModelManager::GetInstance()->GetModel(8));
	World = Object3d::Create(ModelManager::GetInstance()->GetModel(9));
	//SEの初期か
	ClickSe = new Audio();
	ClickSe->Initialize();
	Bgm = new Audio();
	Bgm->Initialize();
	Bgm->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);
	//ポストエフェクトの初期化
	Post = new PostEffect();
	Post->Initialize();
	 CamEyeMove = { 0.0f,0.0f,0.0f };
}

//ステータスセット
void TitleScene::StatusSet()
{
	//カメラの動き
	TitleCamera->MoveEyeVector(CamEyeMove);
	TitleCamera->MoveVector(CamMove);
	//天球のステータスのセット
	Sphere->SetRotation(SphereRot);
	Sphere->SetPosition(SpherePos);
	Sphere->SetScale(SphereScl);
	//地面のステータスのセット
	World->SetPosition(WorldPos);
	World->SetScale(WorldScl);
	//左右のビルのステータスのセット
	for (int i = 0; i < BILLS; i++) {
		Bills[i]->SetScale(BillsScl);
		Bills1[i]->SetScale(BillsScl);
		if (i % 2 == 0) {
			BillsPos = { 100.0f, 0,-300.0f + (100 * i / 2) };
			BillsPos1 = { 200.0f,0,-300.0f + (100 * i / 2) };
			BillsRot = { 0.0f,90.0f,0.0f };
		}
		else if (i % 2 == 1) {
			BillsPos = { -100.0f,0,-300.0f + (100 * i / 2) };
			BillsPos1 = { -200.0f, 0,-300.0f + (100 * i / 2) };
			BillsRot = { 0.0f,270.0f,0.0f };
		}
		Bills[i]->SetRotation(BillsRot);
		Bills[i]->SetPosition(BillsPos);
		Bills1[i]->SetRotation(BillsRot);
		Bills1[i]->SetPosition(BillsPos1);
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
}

//全ての更新処理をまとめる
void TitleScene::AllUpdate()
{
	//左右のビルの更新処理
	for (int i = 0; i < BILLS; i++) {
		Bills[i]->Update({ 0.4f,0.4f,0.5f,1.0f });
		Bills1[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}
	//ポストエフェクトの更新処理
	Post->Update(Post_Col);
	//天球の更新処理
	Sphere->Update();
	//地面の更新処理
	World->Update();
	//カメラの移動先のビルの更新処理
	Start->Update();
}

//更新処理
void TitleScene::Update()
{
	Mouse::GetInstance()->MouseMoveSprite(ReticlePos);
	//カメラのムーブ関数
	CameraMove();
	//カーソルがスプライトの範囲内であるか
	CheckCursorIn(ReticlePos, ClickPos, 500, 75);
	//最初のクリック
	if (CursorIn_F == true && TitleDisplay_F == true) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			TitleSprite_F = false;
			CameraEyeMove_F = true;
			TitleDisplay_F = false;
		}
	}

	//カメラが移動した後の画面
	if (ExplanationPage < 2 && TitleDisplay_F == false&&CameraChange_F==true) {
		if (NextorBack(ReticlePos, ArrowRightPos, 35, 35)) {
			RightTrueIn_F = true;
			//矢印を押された時
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", volume);
				ExplanationPage += 1;
			}
		}
		else {
			RightTrueIn_F = false;
		}
	}

	//ページが0以上であれば
	if (ExplanationPage > 0) {
		if (NextorBack(ReticlePos, ArrowLeftPos, 35, 35)) {
			LeftTrueIn_F = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1))) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", volume);
				ExplanationPage -= 1;
			}
		}
		else {
			LeftTrueIn_F = false;
		}
	}

	//救援ヘリを呼ぶとき
	if (CameraChange_F == true && CursorIn_F == true && ExplanationPage == 2) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (Click_F == true) {
				ClickSe->LoadFile("Resources/Sound/SE/MorseCode.wav", volume);
				Click_F = false;
			}
			FadeOut_F = true;
		}
	}

	//救援ヘリを読んだ後
	if (FadeOut_F == true) {
		Post_Col.x -= SubColor;
		Post_Col.y -= SubColor;
		Post_Col.z -= SubColor;
		Post_Col.w -= SubColor;
		if (Post_Col.w <= 0) {
			//シーン切り替え
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}

	StatusSet();
	AllUpdate();
	//カメラの再計算
	TitleCamera->RecalculationMatrix();
}

//カメラの移動
void TitleScene::CameraMove()
{
	CamMove = { 0.0f,0.0f,0.0f };
	if (CameraEyeMove_F == true && CameraChange_F == false) {
		if ( CamEyeMove.m128_f32[0] >= -1.1) {
			 CamEyeMove.m128_f32[0] -= CameraEyeMoveValue;
			CamMove.m128_f32[0] += CameraMoveValueXandY;
			CamMove.m128_f32[1] -= CameraMoveValueXandY;
			CamMove.m128_f32[2] -= CameraMoveValueZ;
		}
		else {
			CameraChange_F = true;
		}
	}

	if (CameraChange_F == true) {
		 CamEyeMove = { 0.0f,0.0f,0.0f };
	}
}

//カーソルが範囲内に入っているか
void TitleScene::CheckCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY)
{
	if ((check_Pos.x <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) && (check_Pos.y <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		CursorIn_F = true;
	}
	else {
		CursorIn_F = false;
	}
}

//矢印のスプライトの範囲
bool TitleScene::NextorBack(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY)
{
	if ((check_Pos.x <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) && (check_Pos.y <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		return true;
	}

	return false;
}

//描画処理
void TitleScene::Draw(DirectXCommon* dxCommon)
{
	Post->PreDrawScene(dxCommon->GetCmdList());

	Object3d::PreDraw(dxCommon->GetCmdList());
	World->Draw();
	Sphere->Draw();
	for (int i = 0; i < BILLS; i++) {
		Bills[i]->Draw();
		Bills1[i]->Draw();
	}
	Start->Draw();
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	if (TitleSprite_F == true) {
		Title->Draw();
	}
	if (CameraEyeMove_F == false) {
		if (CursorIn_F == false) {
			ClickBefore->Draw();
		}
		else if (CursorIn_F == true) {
			ClickAfter->Draw();
		}
	}
	else if (CameraChange_F == true ) {
		if (ExplanationPage < 2) {
			if (RightTrueIn_F == false) {
				ArrowRight->Draw();
			}
			else {
				ArrowRightTrue->Draw();
			}
		}
		if (ExplanationPage > 0) {
			if (LeftTrueIn_F == false) {
				ArrowLeft->Draw();
			}
			else {
				ArrowLeftTrue->Draw();
			}
		}
		if (ExplanationPage == 0) {
			Explanation->Draw();
		}
		else if (ExplanationPage == 1) {
			Explanation2->Draw();
		}
		else if (ExplanationPage == 2) {
			Explanation3->Draw();
		}
		if (ExplanationPage == 2) {
			if (CursorIn_F == false) {
				SignalBefore->Draw();
			}
			else if (CursorIn_F == true) {
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
	delete Title;
	delete dxCommon;
	delete Post;
	delete TitleCamera;
	delete light;
	delete ArrowLeft;
	delete ArrowRight;
	delete ArrowLeftTrue;
	delete ArrowRightTrue;
	delete Explanation;
	delete Explanation2;
	delete Explanation3;
	delete SignalAfter;
	delete SignalBefore;
	delete ClickSe;
	delete Cursor;
	delete Bgm;

	for (int i = 0; i < BILLS; i++) {
		Bills[i].reset();
		Bills1[i].reset();
	}
	Sphere.reset();
	World.reset();
	Start.reset();
}