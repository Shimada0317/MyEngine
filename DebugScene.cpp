#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{

	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);
	for (int i = 0; i < 9; i++) {
		bull[i] = new Bullet();
		bull[i]->Initialize();
	}
	for (int i = 0; i < 3; i++) {
		rob[i] = std::make_unique<Robot>();
		rob[i]->Initialize();
		allpos[i] = {0.0f + i*1.0f,0.0f,10.0f};
	}
	player = std::make_unique<Player>();
	player->Initalize();

}

void DebugScene::SetPosSclRot()
{
	for (int i = 0; i < 3; i++) {
		rob[i]->SetPosition(allpos[i]);
	}
}

void DebugScene::Update()
{
	SetPosSclRot();
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 3; j++) {
			rob[j]->Update(bull[i]);
			//break;
		}
		//break;
	}
	
	player->Update(bull,Remaining);
	//camera->Update();

}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	for (int j = 0; j < 3; j++) {
		rob[j]->Draw(dxCommon);
	}
	player->ParticleDraw(dxCommon->GetCmdList());
	Object3d::PreDraw(dxCommon->GetCmdList());
	for (int i = 0; i < 9; i++) {
		bull[i]->Draw();
	}
	player->ObjDraw();
	Object3d::PostDraw();
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{
	for (int j = 0; j < 3; j++) {
		rob[j]->Finalize();
	}
	player->Finalize();
}
