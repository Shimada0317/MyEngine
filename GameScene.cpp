#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"SceneManager.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"

using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
	
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	
	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	//モデルの読み込み
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/

	ramieru = Model::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);

	
	
	
	camera->SetEye({ 0,0,90 });
	camera->SetTarget({ 0,1,0 });
	camera->SetDistance({ 10.0f });

	//モデル名を指定してファイル読み込み
	model = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	FbxObject3d::SetDevice(dxComon->GetDev());
	FbxObject3d::SetCamera(camera);
	FbxObject3d::CreateGraphicPipeline();

	Object = new FbxObject3d;
	Object->Initialize();
	Object->SetModel(model);

}

void GameScene::SetPosSclRot()
{
	//プレイヤー
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/

	ramieru3d->SetRotation({ 0,0,0 });
	ramieru3d->SetPosition({ ramieru_pos });
	ramieru3d->SetScale({ 1.0f,1.0f,1.0f });
	
	

	title->SetSize({ 1280.0f,720.0f });
}

void GameScene::Update()
{
	//DirectX毎フレーム処理 ここから
	

	if (Input::GetInstance()->PushKey(DIK_1)) {
		Object->PlayAnimation();
	}
	
	SetPosSclRot();
	camera->Update();
	ramieru3d->Update();
	
	Object->Update();
	

}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	

	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();

	//オブジェクト前処理
	Object3d::PreDraw();
	//player3d->Draw();
	//ramieru3d->Draw();
	//human3d->Draw();
	//オブジェクト後処理
	Object3d::PostDraw();

	Object->Draw(dxCommon->GetCmdList());
}

void GameScene::Finalize()
{
	delete title;
	delete player3d;
}