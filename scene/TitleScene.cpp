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
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);

	light = Light::Create();
	Object3d::SetLight(light);

	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/start.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	Sprite::LoadTexture(2, L"REsources/mark2.png");
	cursor = Sprite::SpriteCreate(2, retpos, spCol, anc);

	Sprite::LoadTexture(3, L"Resources/Click.png");
	clickBefore = Sprite::SpriteCreate(3, ClickPos);

	Sprite::LoadTexture(4, L"Resources/Click2.png");
	clickAfter = Sprite::SpriteCreate(4, ClickPos);

	Sprite::LoadTexture(5, L"Resources/SIGNAL.png");
	SignalBefore = Sprite::SpriteCreate(5, ClickPos);

	Sprite::LoadTexture(6, L"Resources/SIGNAL2.png");
	SignalAfter = Sprite::SpriteCreate(6, ClickPos);

	Sprite::LoadTexture(7, L"Resources/setumei2.png");
	setumei = Sprite::SpriteCreate(7, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, spCol, anc);

	Sprite::LoadTexture(8, L"Resources/setumei.png");
	setumei2 = Sprite::SpriteCreate(8, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, spCol, anc);

	Sprite::LoadTexture(9, L"Resources/setumei3.png");
	setumei3 = Sprite::SpriteCreate(9, { 1280.0f / 2.0f,720.0f / 2.0f - 72.0f }, spCol, anc);

	Sprite::LoadTexture(10, L"Resources/arrowRight.png");
	arrowRight = Sprite::SpriteCreate(10, arrowRightPos);

	Sprite::LoadTexture(11, L"Resources/arrowLeft.png");
	arrowLeft = Sprite::SpriteCreate(11, arrowLeftPos);

	Sprite::LoadTexture(12, L"Resources/arrowRightTrue.png");
	arrowRightTrue = Sprite::SpriteCreate(12, arrowRightPos);

	Sprite::LoadTexture(13, L"Resources/arrowLeftTrue.png");
	arrowLeftTrue = Sprite::SpriteCreate(13, arrowLeftPos);


	billsModel = ObjModel::CreateFromOBJ("bills");
	for (int i = 0; i < BILLS; i++) {
		bills[i] = Object3d::Create(billsModel);
		bills1[i] = Object3d::Create(billsModel);
	}

	spheremodel = ObjModel::CreateFromOBJ("skydome");
	sphere = Object3d::Create(spheremodel);

	worldmodel = ObjModel::CreateFromOBJ("world", true);
	world = Object3d::Create(worldmodel);

	ground = ObjModel::CreateFromOBJ("Field2", true);
	groundObj = Object3d::Create(ground);

	startModel = ObjModel::CreateFromOBJ("bil", true);
	Start = Object3d::Create(startModel);

	click = new Audio();
	click->Initialize();
	MorseCode = new Audio();
	MorseCode->Initialize();
	Bgm = new Audio();
	Bgm->Initialize();
	Bgm->LoopWave("Resources/Sound/BGM/title.wav", 0.75f);

	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	post = new PostEffect();
	post->Initialize();
	camEyeMove = { 0.0f,0.0f,0.0f };
}

void TitleScene::SetPosSclRot()
{
	camera->MoveEyeVector(camEyeMove);
	camera->MoveVector(camMove);

	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition(pos);
	sphere->SetScale({ 4.0f,4.0f,4.0f });

	groundObj->SetPosition(ground_pos);
	groundObj->SetScale({ ground_scl });

	world->SetPosition(worldPos);
	world->SetScale(worldScl);

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

	Start->SetPosition(start_pos);
	Start->SetScale(start_scl);
	Start->SetRotation({ 0.0f,180.0f,0.0f });


	title->SetSize({ titlesize });
	title->SetPosition({ titlepos });

	cursor->SetPosition({ retpos });


}

void TitleScene::Updata()
{
	Action::GetInstance()->PlayerMove2d(titlesize, 1.0f);

	Mouse::GetInstance()->MouseMoveSprite(retpos);
	//DirectX毎フレーム処理 ここから

	camMove = { 0.0f,0.0f,0.0f };

	CameraMove();

	CheckCursorIn(retpos, ClickPos, 500, 75);

	timer += 1;
	if (timer >= 10 && cursorIn == true && titleF == true) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			titleSprite = false;
			cameraEyeMove = true;
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

	if (setumeiSp < 2 && titleF == false&&cameraChange==true) {
		if (NextorBack(retpos, arrowRightPos, 35, 35)) {
			RightTrueIn = true;

			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) && frees == true) {
				click->LoadFile("Resources/Sound/SE/click.wav", 0.8f);
				setumeiSp += 1;
				frees = false;
			}
		}
		else {
			RightTrueIn = false;
		}
	}
	if (setumeiSp > 0) {
		if (NextorBack(retpos, arrowLeftPos, 35, 35)) {
			LeftTrueIn = true;
			if ((Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) && frees == true) {
				click->LoadFile("Resources/Sound/SE/click.wav", 0.8f);
				setumeiSp -= 1;
				frees = false;
			}
		}
		else {
			LeftTrueIn = false;
		}
	}
	if (cameraChange == true && cursorIn == true && setumeiSp == 2) {
		if (Mouse::GetInstance()->PushClick(0) || Mouse::GetInstance()->PushClick(1)) {
			if (clickF == true) {
				click->LoadFile("Resources/Sound/SE/MorseCode.wav", volume);
				clickF = false;
			}
			blackOut = true;

		}
	}
	if (blackOut == true) {
	
		col.x -= 0.01f;
		col.y -= 0.01f;
		col.z -= 0.01f;
		col.w -= 0.01f;
		if (col.w <= 0) {
			//シーン切り替え
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
		}
	}

	for (int i = 0; i < BILLS; i++) {
		bills[i]->Updata({ 0.4f,0.4f,0.5f,1.0f });
		bills1[i]->Updata({ 0.2f,0.2f,0.2f,0.9f });

	}

	post->Update(col);
	SetPosSclRot();
	sphere->Updata();
	groundObj->Updata();
	world->Updata();
	Start->Updata();

	camera->Updata();
}


void TitleScene::CameraMove()
{

	if (cameraEyeMove == true && cameraChange == false) {
		if (camEyeMove.m128_f32[0] >= -1.1) {
			camEyeMove.m128_f32[0] -= 0.01f;
			camMove.m128_f32[0] += 0.4f;
			camMove.m128_f32[1] -= 0.4f;
			camMove.m128_f32[2] -= 0.1f;
		}
		else {
			cameraChange = true;
		}
	}

	if (cameraChange == true) {
		camEyeMove = { 0.0f,0.0f,0.0f };
	}

}

void TitleScene::CheckCursorIn(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY)
{
	if ((checkPos.x <= cursorPos.x && checkPos.x + radX >= cursorPos.x) && (checkPos.y <= cursorPos.y && checkPos.y + radY >= cursorPos.y)) {
		cursorIn = true;
	}
	else {
		cursorIn = false;
	}
}

bool TitleScene::NextorBack(const XMFLOAT2& cursorPos, const XMFLOAT2& checkPos, float radX, float radY)
{
	if ((checkPos.x <= cursorPos.x && checkPos.x + radX >= cursorPos.x) && (checkPos.y <= cursorPos.y && checkPos.y + radY >= cursorPos.y)) {
		return true;
	}

	return false;
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{

	post->PreDrawScene(dxCommon->GetCmdList());



	Object3d::PreDraw(dxCommon->GetCmdList());
	world->Draw();
	sphere->Draw();
	//groundObj->Draw();
	for (int i = 0; i < BILLS; i++) {
		bills[i]->Draw();
		bills1[i]->Draw();
	}
	Start->Draw();

	////human3d->Draw();
	////オブジェクト後処理
	Object3d::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	if (titleSprite == true) {
		title->Draw();
	}
	if (cameraEyeMove == false) {
		if (cursorIn == false) {
			clickBefore->Draw();
		}
		else if (cursorIn == true) {
			clickAfter->Draw();
		}
	}
	else if (cameraChange == true && firework == false) {
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
			if (cursorIn == false) {
				SignalBefore->Draw();
			}
			else if (cursorIn == true) {
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
	delete camera;
	delete light;
	delete billsModel;
	delete spheremodel;
	delete worldmodel;
	delete ground;
	delete arrowLeft;
	delete arrowRight;
	delete arrowLeftTrue;
	delete arrowRightTrue;
	delete setumei;
	delete setumei2;
	delete setumei3;
	delete SignalAfter;
	delete SignalBefore;
	delete startModel;
	delete click;
	delete MorseCode;
	delete cursor;
	delete Bgm;

	for (int i = 0; i < BILLS; i++) {
		bills[i].reset();
		bills1[i].reset();
	}
	sphere.reset();
	world.reset();
	groundObj.reset();
	Start.reset();
}