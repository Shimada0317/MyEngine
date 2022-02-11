#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

void GameScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);

	this->dxCommon = dxCommon;

	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	////スプライトの読み込み

	Sprite::LoadTexture(1, L"Resources/title.png");


	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);

	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });


	//モデルの読み込み
	playermodel = Model::LoadFromObJ("player");

	//オブジェクトのクリエイト
	player3d = Object3d::Create();

	//モデルのセット
	player3d->SetModel(playermodel);

}


void GameScene::GameSceneInitialize()
{
	//プレイヤー
	player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});

	title->SetSize({ 1280.0f,720.0f });
}

void GameScene::Update(DirectXCommon* dxCommon,Input* input)
{
	//DirectX毎フレーム処理 ここから
	
	this->dxCommon = dxCommon;
	//this->audio = audio;

	//audio->LoadFile("Resources/digitalworld.wav", vol[0]);

	player3d->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	// コマンドリストの取得
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//dxCommon->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	debugText->DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	//オブジェクト前処理
	Object3d::PreDraw(dxCommon->GetCmdList());
	player3d->Draw();
	//オブジェクト後処理
	Object3d::PostDraw();

}

void GameScene::Finalize()
{
	delete title;
	delete player3d;
}