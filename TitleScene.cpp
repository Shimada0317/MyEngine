#include "TitleScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

void TitleScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);

	this->dxCommon = dxCommon;

	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height, dxCommon->GetCmdList());
	////スプライトの読み込み

	Sprite::LoadTexture(1, L"Resources/title.png");


	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);

	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	//モデルの読み込み
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/

	ramieru = Model::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);

	/*human = Model::LoadFromObJ("human");
	human3d = Object3d::Create();
	human3d->SetModel(human);*/

}


void TitleScene::TitleSceneInitialize()
{
	//プレイヤー
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/

	ramieru3d->SetRotation({ 0,90,0 });
	ramieru3d->SetPosition({ 1,1,1 });
	ramieru3d->SetScale({ 1.0f,1.0f,1.0f });

	/*human3d->SetRotation({ 0,0,0 });
	human3d->SetPosition({ -1,-1,-1 });
	human3d->SetScale({ 0.5f,0.5f,0.5f });*/

	title->SetSize({ 1280.0f,720.0f });
}

void TitleScene::Update(DirectXCommon* dxCommon, Input* input)
{
	//DirectX毎フレーム処理 ここから

	this->dxCommon = dxCommon;
	//this->audio = audio;

	//audio->LoadFile("Resources/digitalworld.wav", vol[0]);

	//player3d->Update();
	ramieru3d->Update();
	//human3d->Update();
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{
	// コマンドリストの取得
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//dxCommon->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Sprite::PreDraw(dxCommon->GetCmdList());
	//title->Draw();
	Sprite::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	debugText->DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	//オブジェクト前処理
	Object3d::PreDraw();
	//player3d->Draw();
	ramieru3d->Draw();
	//human3d->Draw();
	//オブジェクト後処理
	Object3d::PostDraw();

}

void TitleScene::Finalize()
{
	delete title;
	delete player3d;
}