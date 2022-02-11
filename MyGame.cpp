#include "MyGame.h"
#include"GameScene.h"

void MyGame::Initialize()
{
	//基底クラスの初期化
	Framework::Initialize();
	scene_ = new GameScene();
	scene_->Initialize(dxCommon);
	scene_->GameSceneInitialize();
}

void MyGame::Update()
{
	//基底クラスの更新
	Framework::Update();

	scene_->Update(dxCommon, input);
}

void MyGame::Draw()
{
	//描画処理
	dxCommon->PreDraw();

	scene_->Draw(dxCommon);
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	//基底クラスの終了
	Framework::Finalize();
	scene_->Finalize();
	//delete contoroller;
	//delete model;
}
