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
	Sprite::LoadTexture(1, L"Resources/tst.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });


	billsModel = ObjModel::CreateFromOBJ("bills");
	for (int i = 0; i < BILLS; i++) {
		bills[i] = Object3d::Create(billsModel);
		bills1[i] = Object3d::Create(billsModel);
	}

	spheremodel = ObjModel::CreateFromOBJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(spheremodel);

	worldmodel = ObjModel::CreateFromOBJ("world", true);
	world = Object3d::Create(worldmodel);

	ground = ObjModel::CreateFromOBJ("Field2", true);
	groundObj = Object3d::Create(ground);

	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	post = new PostEffect();
	post->Initialize();


}

void TitleScene::SetPosSclRot()
{
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

	title->SetSize({ titlesize });
	title->SetPosition({ titlepos });
}

void TitleScene::Updata()
{
	Action::GetInstance()->PlayerMove2d(titlesize,1.0f);

	//DirectX毎フレーム処理 ここから
	timer += 1;
	if (timer >= 10) {
		if (Input::GetInstance()->PushClick(0)|| Input::GetInstance()->PushClick(1)) {
			//シーン切り替え
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
			timer = 0;
		}
	}
	post->Update(col);
	
	camMove = { 0.0f,0.0f,0.0f };
	if (Input::GetInstance()->PushKey(DIK_A)) {
		camMove.m128_f32[0] = -1.0f;
	}
	else if (Input::GetInstance()->PushKey(DIK_D)) {
		camMove.m128_f32[0] = 1.0f;
	}

	if (Input::GetInstance()->PushKey(DIK_S)) {
		camMove.m128_f32[1] = -1.0f;
	}
	else if (Input::GetInstance()->PushKey(DIK_W)) {
		camMove.m128_f32[1] = 1.0f;
	}

	if (Input::GetInstance()->PushKey(DIK_Q)) {
		camMove.m128_f32[2] = -1.0f;
	}
	else if (Input::GetInstance()->PushKey(DIK_E)) {
		camMove.m128_f32[2] = 1.0f;
	}

	for (int i = 0; i < BILLS; i++) {
		bills[i]->Updata({ 1.0f,1.0f,0.7f,0.7f });
		bills1[i]->Updata({ 1.0f,0.5f,0.0f,0.9f });
	}

	SetPosSclRot();
	sphere->Updata();
	groundObj->Updata();
	world->Updata();
	camera->Updata();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{

	post->PreDrawScene(dxCommon->GetCmdList());
	/*Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();*/

	

	Object3d::PreDraw(dxCommon->GetCmdList());
	world->Draw();
	sphere->Draw();
	//groundObj->Draw();
	for (int i = 0; i < BILLS; i++) {
		bills[i]->Draw();
		bills1[i]->Draw();
	}

	////human3d->Draw();
	////オブジェクト後処理
	Object3d::PostDraw();
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
}