#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
#include"SceneManager.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"


using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
	
	//Sprite::LoadTexture(100, L"Resources/background.png");
	postEffect = new PostEffect();
	postEffect->Initialize();

	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);
	
	////スプライトの読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	
	//postEffect->InitializeSprite();

	//モデルの読み込み
	playermodel = ObjModel::LoadFromObJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(playermodel);

	ramieru = ObjModel::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);
	ramieru3d->SetCamera({ camera });

	ground = ObjModel::LoadFromObJ("ground");
	groundObj = Object3d::Create();
	groundObj->SetModel(ground);
	
	

	//モデル名を指定してファイル読み込み
	model = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	FbxObject3d::SetDevice(dxComon->GetDev());
	FbxObject3d::SetCamera(camera);
	FbxObject3d::CreateGraphicPipeline();

	Object = new FbxObject3d;
	Object->Initialize();
	Object->SetModel(model);

	particle = ParticleManager::Create();
	particle->Update();

}

void GameScene::SetPosSclRot()
{
	//プレイヤー
	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition({ pos });
	sphere->SetScale({4.0f,4.0f,4.0f});


	ramieru3d->SetRotation({ ramieru_rot });
	ramieru3d->SetPosition({ ramieru_pos });
	ramieru3d->SetScale({ ramieru_scl });
	ramieru3d->SetEye({ cameraEye });
	ramieru3d->SetTarget({ cameraTarget });
	ramieru3d->CameraMoveVector({ ramieru_pos });

	camera->SetDistance({ cameradistance });
	camera->SetEye({ cameraEye });
	camera->SetTarget({ cameraTarget });

	groundObj->SetPosition({ ground_pos });
	groundObj->SetScale({ ground_scl });

	Object->SetRotation({ obj_rot });
	Object->SetPosition({ obj_pos });
	Object->SetRotation({ obj_rot });

	title->SetSize({ screen_size });

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

		particle->Add(60, pos, vel, acc, 1.0f, 0.0f);
		break;
	}
}

void GameScene::AllUpdate()
{
	camera->Update();
	ramieru3d->Update();
	sphere->Update();
	Object->Update();
	groundObj->Update();
	particle->Update();

}

void GameScene::Update()
{
	//DirectX毎フレーム処理 ここから
	if (Input::GetInstance()->PushKey(DIK_1)) {
		Object->PlayAnimation();
	}

	//Action::GetInstance()->PlayerMove3d(cameraEye, 0.5f);
	//Action::GetInstance()->PlayerMove3d(cameraTarget, 0.2f);
	Action::GetInstance()->PlayerMove3d(ramieru_pos, 0.5f);
	Action::GetInstance()->PlayerJump(ramieru_pos,JumpFlag);

	SetPosSclRot();
	AllUpdate();
}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{

	ParticleManager::PreDraw(dxCommon->GetCmdList());
	particle->Draw();
	ParticleManager::PostDraw();
	////オブジェクト前処理
	Object3d::PreDraw();
	sphere->Draw();
	groundObj->Draw();
	ramieru3d->Draw();
	////human3d->Draw();
	////オブジェクト後処理
	Object3d::PostDraw();


	Object->Draw(dxCommon->GetCmdList());
}

void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();
}

void GameScene::ImgDraw()
{
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.7f, 0.7f,1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.3f, 0.1f, 0.0f));
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Begin("Rendering Test Menu");
	bool blnCk = false;
	//フラグを手動で切りたい時
	ImGui::Checkbox("ramieruposition",&blnCk);
	//スライダーで動きをいじりたいとき
	ImGui::SliderFloat("ramieru_pos.x", &ramieru_pos.x, -100.0f, 100.0f);
	ImGui::SliderFloat("ramieru_pos.y", &ramieru_pos.y, -100.0f, 100.0f);
	ImGui::SliderFloat("ramieru_pos.z", &ramieru_pos.z, -100.0f, 100.0f);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	postEffect->PreDrawScene(dxCommon->GetCmdList());
	//SpriteDraw(dxCommon);
	ObjDraw(dxCommon);
	postEffect->PostDrawScene(dxCommon->GetCmdList());
	//描画前処理
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());
	ImgDraw();
	//描画後処理
	dxCommon->PostDraw();
}

void GameScene::Finalize()
{
	delete title;
	delete sphere;
	delete groundObj;
	delete ramieru3d;
	//delete sphere;
	delete winApp;
	delete dxCommon;
}