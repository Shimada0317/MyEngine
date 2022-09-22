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


	//camera->SetTarget({ Target_pos.x,Target_pos.y,allpos.m128_f32[2] });
	//camera->SetEye({ Eye_pos });
	//camera->SetDistance(5);
}

void DebugScene::Update()
{
	rob->Update();

	//camera->Update();

}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Object3d::PreDraw(dxCommon->GetCmdList());
	rob->Draw();
	Object3d::PostDraw();
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{
	//rob->Finalize();
}
