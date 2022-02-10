#include "MyGame.h"

void MyGame::Initialize()
{
	//基底クラスの初期化
	Framework::Initialize();

	gamescene->Initialize(dxCommon);
	gamescene->GameSceneInitialize();
}

void MyGame::Update()
{
	//基底クラスの更新
	Framework::Update();

	gamescene->Update(dxCommon, input);
}

void MyGame::Draw()
{
	//描画処理
	dxCommon->PreDraw();

	gamescene->Draw(dxCommon);
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	//基底クラスの終了
	Framework::Finalize();
	
	//delete contoroller;
	//delete model;
}
