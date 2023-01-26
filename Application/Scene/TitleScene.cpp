#include "TitleScene.h"
#include"SceneManager.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"GameScene.h"
#include"Mouse.h"

using namespace DirectX;

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
	Title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	Sprite::LoadTexture(2, L"REsources/mark2.png");
	Cursor = Sprite::SpriteCreate(2, ReticlePos, SpriteCol, Anchorpoint);

	Sprite::LoadTexture(3, L"Resources/Click.png");
	ClickBefore = Sprite::SpriteCreate(3, ClickPos);

	Sprite::LoadTexture(4, L"Resources/Click2.png");
	ClickAfter = Sprite::SpriteCreate(4, ClickPos);

	Sprite::LoadTexture(5, L"Resources/SIGNAL.png");
	SignalBefore = Sprite::SpriteCreate(5, ClickPos);

	Sprite::LoadTexture(6, L"Resources/SIGNAL2.png");
	SignalAfter = Sprite::SpriteCreate(6, ClickPos);

	Sprite::LoadTexture(7, L"Resources/setumei2.png");
	Explanation = Sprite::SpriteCreate(7, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, SpriteCol, Anchorpoint);

	Sprite::LoadTexture(8, L"Resources/setumei.png");
	Explanation2 = Sprite::SpriteCreate(8, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, SpriteCol, Anchorpoint);

	Sprite::LoadTexture(9, L"Resources/setumei3.png");
	Explanation3 = Sprite::SpriteCreate(9, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, SpriteCol, Anchorpoint);

	Sprite::LoadTexture(10, L"Resources/arrowRight.png");
	ArrowRight = Sprite::SpriteCreate(10, ArrowRightPos);

	Sprite::LoadTexture(11, L"Resources/arrowLeft.png");
	ArrowLeft = Sprite::SpriteCreate(11, ArrowLeftPos);

	Sprite::LoadTexture(12, L"Resources/arrowRightTrue.png");
	ArrowRightTrue = Sprite::SpriteCreate(12, ArrowRightPos);

	Sprite::LoadTexture(13, L"Resources/arrowLeftTrue.png");
	ArrowLeftTrue = Sprite::SpriteCreate(13, ArrowLeftPos);


	BillsModel = ObjModel::CreateFromOBJ("Bills");
	for (int i = 0; i < BILLS; i++) {
		Bills[i] = Object3d::Create(BillsModel);
		Bills1[i] = Object3d::Create(BillsModel);
	}

	Spheremodel = ObjModel::CreateFromOBJ("skydome");
	Sphere = Object3d::Create(Spheremodel);

	Worldmodel = ObjModel::CreateFromOBJ("World", true);
	World = Object3d::Create(Worldmodel);

	StartModel = ObjModel::CreateFromOBJ("bil", true);
	Start = Object3d::Create(StartModel);

	ClickSe = new Audio();
	ClickSe->Initialize();
	Bgm = new Audio();
	Bgm->Initialize();
	Bgm->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);

	Post = new PostEffect();
	Post->Initialize();
	 CamEyeMove = { 0.0f,0.0f,0.0f };
}

//ステータスセット
void TitleScene::StatusSet()
{
	TitleCamera->MoveEyeVector( CamEyeMove);
	TitleCamera->MoveVector(CamMove);

	Sphere->SetRotation(SphereRot);
	Sphere->SetPosition(SpherePos);
	Sphere->SetScale(SphereScl);

	World->SetPosition(WorldPos);
	World->SetScale(WorldScl);

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

	Start->SetPosition(StartPos);
	Start->SetScale(StartScl);
	Start->SetRotation({ 0.0f,180.0f,0.0f });

	Title->SetSize({ TitleSize });
	Title->SetPosition({ TitlePos });

	Cursor->SetPosition({ ReticlePos });
}

void TitleScene::AllUpdate()
{
	for (int i = 0; i < BILLS; i++) {
		Bills[i]->Update({ 0.4f,0.4f,0.5f,1.0f });
		Bills1[i]->Update({ 0.2f,0.2f,0.2f,0.9f });
	}

	Post->Update(Post_Col);
	StatusSet();
	Sphere->Update();
	World->Update();
	Start->Update();
}

//更新処理
void TitleScene::Update()
{
	Action::GetInstance()->PlayerMove2d(TitleSize, 1.0f);

	Mouse::GetInstance()->MouseMoveSprite(ReticlePos);
	//DirectX毎フレーム処理 ここから

	CamMove = { 0.0f,0.0f,0.0f };

	CameraMove();

	CheckCursorIn(ReticlePos, ClickPos, 500, 75);
	//最初のクリック
	if (CursorIn_F == true && TitleDisplay_F == true) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			TitleSprite_F = false;
			CameraEyeMove_F = true;
			TitleDisplay_F = false;
		}
	}

	if (CantClick_F == false) {
		CantClickTimer += 0.1f;
		if (CantClickTimer >= 1) {
			CantClickTimer = 0.0f;
			CantClick_F = true;
		}
	}
	//カメラが移動した後の画面
	if (SetumeiPage < 2 && TitleDisplay_F == false&&CameraChange_F==true) {
		if (NextorBack(ReticlePos, ArrowRightPos, 35, 35)) {
			RightTrueIn_F = true;
			//矢印を押された時
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) && CantClick_F == true) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", volume);
				SetumeiPage += 1;
				CantClick_F = false;
			}
		}
		else {
			RightTrueIn_F = false;
		}
	}

	if (SetumeiPage > 0) {
		if (NextorBack(ReticlePos, ArrowLeftPos, 35, 35)) {
			LeftTrueIn_F = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) && CantClick_F == true) {
				ClickSe->LoadFile("Resources/Sound/SE/click.wav", volume);
				SetumeiPage -= 1;
				CantClick_F = false;
			}
		}
		else {
			LeftTrueIn_F = false;
		}
	}
	if (CameraChange_F == true && CursorIn_F == true && SetumeiPage == 2) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (Click_F == true) {
				ClickSe->LoadFile("Resources/Sound/SE/MorseCode.wav", volume);
				Click_F = false;
			}
			FadeOut_F = true;

		}
	}
	if (FadeOut_F == true) {
	
		Post_Col.x -= 0.01f;
		Post_Col.y -= 0.01f;
		Post_Col.z -= 0.01f;
		Post_Col.w -= 0.01f;
		if (Post_Col.w <= 0) {
			//シーン切り替え
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}

	AllUpdate();

	TitleCamera->RecalculationMatrix();
}

//カメラの移動
void TitleScene::CameraMove()
{
	if (CameraEyeMove_F == true && CameraChange_F == false) {
		if ( CamEyeMove.m128_f32[0] >= -1.1) {
			 CamEyeMove.m128_f32[0] -= 0.01f;
			CamMove.m128_f32[0] += 0.4f;
			CamMove.m128_f32[1] -= 0.4f;
			CamMove.m128_f32[2] -= 0.1f;
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

	////オブジェクト後処理
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
		if (SetumeiPage < 2) {
			if (RightTrueIn_F == false) {
				ArrowRight->Draw();
			}
			else {
				ArrowRightTrue->Draw();
			}
		}
		if (SetumeiPage > 0) {
			if (LeftTrueIn_F == false) {
				ArrowLeft->Draw();
			}
			else {
				ArrowLeftTrue->Draw();
			}
		}
		if (SetumeiPage == 0) {
			Explanation->Draw();
		}
		else if (SetumeiPage == 1) {
			Explanation2->Draw();
		}
		else if (SetumeiPage == 2) {
			Explanation3->Draw();
		}
		if (SetumeiPage == 2) {
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
	delete BillsModel;
	delete Spheremodel;
	delete Worldmodel;
	delete Ground;
	delete ArrowLeft;
	delete ArrowRight;
	delete ArrowLeftTrue;
	delete ArrowRightTrue;
	delete Explanation;
	delete Explanation2;
	delete Explanation3;
	delete SignalAfter;
	delete SignalBefore;
	delete StartModel;
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