#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"SceneManager.h"
#include"FbxLoader.h"

using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize()
{
	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/GameScene.png");
	Sprite::LoadTexture(2, L"Resources/circle.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });
	sprite = Sprite::SpriteCreate(2, { 1.0f,1.0f });


	//モデルの読み込み
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/

	ramieru = Model::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);

	//モデル名を指定してファイル読み込み
	FbxLoader::GetInstance()->LoadModelFromFile("cube");
}

void GameScene::SetPosSclRot()
{
	//プレイヤー
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/

	ramieru3d->SetRotation({ 0,0,0 });
	ramieru3d->SetPosition({ ramieru_pos });
	ramieru3d->SetScale({ramieru_scl});

	title->SetSize({ 1280.0f,720.0f });
	sprite->SetSize({ 64.0f,64.0f });
	
}

void GameScene::Update()
{
	//DirectX毎フレーム処理 ここから
	
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		GravityF = 1;
	}

	if (GravityF == 1) {
		//自由落下
		t += 0.5;
		//ramieru_pos.y = -0.5 * g * t * t;
		//投げ上げ?
		//Speed = -g * t + Start_Speed;
		Speed = fricition * t;
		ramieru_pos.x = ramieru_pos.x+(SpeedX-fricition);
		//ramieru_pos.y = (-0.5f * g * t * t) + (Start_Speed * t);
	}

	

	SetPosSclRot();
	ramieru3d->Update();

}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	

	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	sprite->Draw();
	Sprite::PostDraw();

	//オブジェクト前処理
	Object3d::PreDraw();
	//player3d->Draw();
	ramieru3d->Draw();
	//human3d->Draw();
	//オブジェクト後処理
	Object3d::PostDraw();

}

void GameScene::Finalize()
{
	delete title;
	delete sprite;
	delete player3d;
}