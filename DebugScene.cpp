#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{

	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);

	bull = new Bullet();
	bull->Initialize();

	rob = std::make_unique<Robot>();
	rob->Initialize();

	player = std::make_unique<Player>();
	player->Initalize();

}

void DebugScene::SetPosSclRot()
{

}

void DebugScene::Update()
{
	SetPosSclRot();
	rob->Update(bull);
	player->Update(bull);
	//camera->Update();

}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	rob->Draw(dxCommon);
	player->ParticleDraw(dxCommon->GetCmdList());
	Object3d::PreDraw(dxCommon->GetCmdList());
	bull->Draw();
	player->ObjDraw();
	Object3d::PostDraw();
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{
	//rob->Finalize();
}
