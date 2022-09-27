#include "DebugScene.h"

DebugScene::DebugScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{
}

void DebugScene::Initialize(DirectXCommon* dxComon)
{

	//camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//Object3d::SetCamera(camera);


	part = ParticleManager::Create();
	//part->Update();

	rob = std::make_unique<Robot>();
	rob->Initialize();

}

void DebugScene::SetPosSclRot()
{


	//camera->SetTarget({ Target_pos.x,Target_pos.y,allpos.m128_f32[2] });
	//camera->SetEye({ Eye_pos });
	//camera->SetDistance(5);

	for (int i = 0; i < 1000; i++) {
		const float rnd_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		part->Add(60, pos, vel, acc, 1.0f, 0.0f);
		break;
	}
}

void DebugScene::Update()
{
	SetPosSclRot();
	part->Update();
	rob->Update();
	//camera->Update();

}

void DebugScene::Draw(DirectXCommon* dxCommon)
{
	dxCommon->PreDraw();
	Object3d::PreDraw(dxCommon->GetCmdList());
	rob->Draw();
	Object3d::PostDraw();
	ParticleManager::PreDraw(dxCommon->GetCmdList());
	part->Draw();
	ParticleManager::PostDraw();

	dxCommon->PostDraw();
}

void DebugScene::Finalize()
{
	//rob->Finalize();
}
