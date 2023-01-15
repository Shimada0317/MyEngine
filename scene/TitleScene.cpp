#include "TitleScene.h"
#include"SceneManager.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"GameScene.h"

using namespace DirectX;

TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void TitleScene::Initialize(DirectXCommon* dxComon)
{
	TitleCamera = new Camera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(TitleCamera);

	light = Light::Create();
	Object3d::SetLight(light);

	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/start.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	Sprite::LoadTexture(2, L"REsources/mark2.png");
	cursor = Sprite::SpriteCreate(2, Reticle_Pos, Sprite_Col, Anchorpoint);

	Sprite::LoadTexture(3, L"Resources/Click.png");
	clickBefore = Sprite::SpriteCreate(3, Click_Pos);

	Sprite::LoadTexture(4, L"Resources/Click2.png");
	clickAfter = Sprite::SpriteCreate(4, Click_Pos);

	Sprite::LoadTexture(5, L"Resources/SIGNAL.png");
	SignalBefore = Sprite::SpriteCreate(5, Click_Pos);

	Sprite::LoadTexture(6, L"Resources/SIGNAL2.png");
	SignalAfter = Sprite::SpriteCreate(6, Click_Pos);

	Sprite::LoadTexture(7, L"Resources/setumei2.png");
	setumei = Sprite::SpriteCreate(7, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, Sprite_Col, Anchorpoint);

	Sprite::LoadTexture(8, L"Resources/setumei.png");
	setumei2 = Sprite::SpriteCreate(8, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, Sprite_Col, Anchorpoint);

	Sprite::LoadTexture(9, L"Resources/setumei3.png");
	setumei3 = Sprite::SpriteCreate(9, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, Sprite_Col, Anchorpoint);

	Sprite::LoadTexture(10, L"Resources/arrowRight.png");
	arrowRight = Sprite::SpriteCreate(10, ArrowRight_Pos);

	Sprite::LoadTexture(11, L"Resources/arrowLeft.png");
	arrowLeft = Sprite::SpriteCreate(11, ArrowLeft_Pos);

	Sprite::LoadTexture(12, L"Resources/arrowRightTrue.png");
	arrowRightTrue = Sprite::SpriteCreate(12, ArrowRight_Pos);

	Sprite::LoadTexture(13, L"Resources/arrowLeftTrue.png");
	arrowLeftTrue = Sprite::SpriteCreate(13, ArrowLeft_Pos);


	BillsModel = ObjModel::CreateFromOBJ("Bills");
	for (int i = 0; i < BILLS; i++) {
		Bills[i] = Object3d::Create(BillsModel);
		Bills1[i] = Object3d::Create(BillsModel);
	}

	Spheremodel = ObjModel::CreateFromOBJ("skydome");
	Sphere = Object3d::Create(Spheremodel);

	Worldmodel = ObjModel::CreateFromOBJ("World", true);
	World = Object3d::Create(Worldmodel);

	Ground = ObjModel::CreateFromOBJ("Field2", true);
	GroundObj = Object3d::Create(Ground);

	StartModel = ObjModel::CreateFromOBJ("bil", true);
	Start = Object3d::Create(StartModel);

	Click = new Audio();
	Click->Initialize();
	MorseCode = new Audio();
	MorseCode->Initialize();
	Bgm = new Audio();
	Bgm->Initialize();
	Bgm->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);

	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	post = new PostEffect();
	post->Initialize();
	 CamEye_Move = { 0.0f,0.0f,0.0f };
}

void TitleScene::SetPosSclRot()
{
	TitleCamera->MoveEyeVector( CamEye_Move);
	TitleCamera->MoveVector(Cam_Move);

	Sphere->SetRotation(Sphere_Rot);
	Sphere->SetPosition(Sphere_Pos);
	Sphere->SetScale(Sphere_Scl);

	GroundObj->SetPosition(Ground_Pos);
	GroundObj->SetScale({ Ground_Scl });

	World->SetPosition(World_Pos);
	World->SetScale(World_Scl);

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

	Start->SetPosition(Start_Pos);
	Start->SetScale(Start_Scl);
	Start->SetRotation({ 0.0f,180.0f,0.0f });


	title->SetSize({ Title_Size });
	title->SetPosition({ Title_Pos });

	cursor->SetPosition({ Reticle_Pos });


}

void TitleScene::Updata()
{
	Action::GetInstance()->PlayerMove2d(Title_Size, 1.0f);

	Mouse::GetInstance()->MouseMoveSprite(Reticle_Pos);
	//DirectX毎フレーム処理 ここから

	Cam_Move = { 0.0f,0.0f,0.0f };

	CameraMove();

	CheckCursorIn(Reticle_Pos, Click_Pos, 500, 75);

	if (CursorIn_F == true && titleF == true) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			titleSprite = false;
			CameraEyeMove_F = true;
			titleF = false;
		}
	}

	if (frees == false) {
		freesTimer += 0.1f;
		if (freesTimer >= 1) {
			freesTimer = 0.0f;
			frees = true;
			
		}
	}

	if (setumeiSp < 2 && titleF == false&&CameraChange_F==true) {
		if (NextorBack(Reticle_Pos, ArrowRight_Pos, 35, 35)) {
			RightTrueIn = true;

			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) && frees == true) {
				Click->LoadFile("Resources/Sound/SE/Click.wav", volume);
				setumeiSp += 1;
				frees = false;
			}
		}
		else {
			RightTrueIn = false;
		}
	}
	if (setumeiSp > 0) {
		if (NextorBack(Reticle_Pos, ArrowLeft_Pos, 35, 35)) {
			LeftTrueIn = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) && frees == true) {
				Click->LoadFile("Resources/Sound/SE/Click.wav", volume);
				setumeiSp -= 1;
				frees = false;
			}
		}
		else {
			LeftTrueIn = false;
		}
	}
	if (CameraChange_F == true && CursorIn_F == true && setumeiSp == 2) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (clickF == true) {
				Click->LoadFile("Resources/Sound/SE/MorseCode.wav", volume);
				clickF = false;
			}
			blackOut = true;

		}
	}
	if (blackOut == true) {
	
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

	for (int i = 0; i < BILLS; i++) {
		Bills[i]->Updata({ 0.4f,0.4f,0.5f,1.0f });
		Bills1[i]->Updata({ 0.2f,0.2f,0.2f,0.9f });

	}

	post->Update(Post_Col);
	SetPosSclRot();
	Sphere->Updata();
	GroundObj->Updata();
	World->Updata();
	Start->Updata();

	TitleCamera->Updata();
}


void TitleScene::CameraMove()
{

	if (CameraEyeMove_F == true && CameraChange_F == false) {
		if ( CamEye_Move.m128_f32[0] >= -1.1) {
			 CamEye_Move.m128_f32[0] -= 0.01f;
			Cam_Move.m128_f32[0] += 0.4f;
			Cam_Move.m128_f32[1] -= 0.4f;
			Cam_Move.m128_f32[2] -= 0.1f;
		}
		else {
			CameraChange_F = true;
		}
	}

	if (CameraChange_F == true) {
		 CamEye_Move = { 0.0f,0.0f,0.0f };
	}

}

void TitleScene::CheckCursorIn(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY)
{
	if ((check_Pos.x <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) && (check_Pos.y <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		CursorIn_F = true;
	}
	else {
		CursorIn_F = false;
	}
}

bool TitleScene::NextorBack(const XMFLOAT2& cursor_Pos, const XMFLOAT2& check_Pos, float radX, float radY)
{
	if ((check_Pos.x <= cursor_Pos.x && check_Pos.x + radX >= cursor_Pos.x) && (check_Pos.y <= cursor_Pos.y && check_Pos.y + radY >= cursor_Pos.y)) {
		return true;
	}

	return false;
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	post->PreDrawScene(dxCommon->GetCmdList());

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
	if (titleSprite == true) {
		title->Draw();
	}
	if (CameraEyeMove_F == false) {
		if (CursorIn_F == false) {
			clickBefore->Draw();
		}
		else if (CursorIn_F == true) {
			clickAfter->Draw();
		}
	}
	else if (CameraChange_F == true && firework == false) {
		if (setumeiSp < 2) {
			if (RightTrueIn == false) {
				arrowRight->Draw();
			}
			else {
				arrowRightTrue->Draw();
			}
		}
		if (setumeiSp > 0) {
			if (LeftTrueIn == false) {
				arrowLeft->Draw();
			}
			else {
				arrowLeftTrue->Draw();
			}
		}
		if (setumeiSp == 0) {
			setumei->Draw();
		}
		else if (setumeiSp == 1) {
			setumei2->Draw();
		}
		else if (setumeiSp == 2) {
			setumei3->Draw();
		}
		if (setumeiSp == 2) {
			if (CursorIn_F == false) {
				SignalBefore->Draw();
			}
			else if (CursorIn_F == true) {
				SignalAfter->Draw();
			}
		}
	}
	cursor->Draw();
	Sprite::PostDraw();
	post->PostDrawScene(dxCommon->GetCmdList());

	dxCommon->PreDraw();
	post->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}

void TitleScene::Finalize()
{
	delete title;
	delete dxCommon;
	delete post;
	delete TitleCamera;
	delete light;
	delete BillsModel;
	delete Spheremodel;
	delete Worldmodel;
	delete Ground;
	delete arrowLeft;
	delete arrowRight;
	delete arrowLeftTrue;
	delete arrowRightTrue;
	delete setumei;
	delete setumei2;
	delete setumei3;
	delete SignalAfter;
	delete SignalBefore;
	delete StartModel;
	delete Click;
	delete MorseCode;
	delete cursor;
	delete Bgm;

	for (int i = 0; i < BILLS; i++) {
		Bills[i].reset();
		Bills1[i].reset();
	}
	Sphere.reset();
	World.reset();
	GroundObj.reset();
	Start.reset();
}