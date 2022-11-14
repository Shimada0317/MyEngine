#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{

	//mid = std::make_unique<middle>();
	//mid->Initialize();

	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);
	/*for (int i = 0; i < 9; i++) {
		bull[i] = new Bullet();
		bull[i]->Initialize();
	}
	for (int i = 0; i < 3; i++) {
		rob[i] = std::make_unique<Robot>();
		rob[i]->Initialize();
		allpos[i] = {0.0f + i*1.0f,0.0f,10.0f};
	}*/
	//player = std::make_unique<Player>();
	//player->Initalize();

	bomodel = ObjModel::CreateFromOBJ("block0");
	bo = Object3d::Create(bomodel);
}

void DebugScene::SetPosSclRot()
{

	bo->SetPosition(position);
}

void DebugScene::Updata()
{
	SetPosSclRot();

	Action::GetInstance()->PlayerMove3d(position);
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		jump = true;
	
	}
	if (jump == true) {
		position.m128_f32[1] = position.m128_f32[1] + addgrav;
		addgrav -= 0.02f;
	}

	//for (int i = 0; i < 9; i++) {
	//	for (int j = 0; j < 3; j++) {
	//		rob[j]->Update(bull[i]);
	//		//break;
	//	}
	//	//break;
	//}
	//
	bo->Updata();
	//camera->Update();
	//mid->Updata();
}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	/*for (int j = 0; j < 3; j++) {
		rob[j]->Draw(dxCommon);
	}*/
	//player->ParticleDraw(dxCommon->GetCmdList());
	Object3d::PreDraw(dxCommon->GetCmdList());
	bo->Draw();

	Object3d::PostDraw();
	//mid->Draw(dxCommon);
	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{
	//for (int j = 0; j < 3; j++) {
	//	rob[j]->Finalize();
	//}
	//player->Finalize();
	///mid->Fainalize();
}
