#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{

	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);

	rob = std::make_unique<Robot>();
	rob->Initialize();

}

void DebugScene::SetPosSclRot()
{

}

void DebugScene::Update()
{
	SetPosSclRot();
	rob->Update();
	//camera->Update();

}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Object3d::PreDraw(dxCommon->GetCmdList());
	rob->Draw(dxCommon);
	Object3d::PostDraw();

	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{
	//rob->Finalize();
}
