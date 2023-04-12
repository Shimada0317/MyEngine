#include "Framework.h"
#include"FbxLoader.h"
#include"ModelManager.h"


void Framework::Run()
{
	//初期化
	Initialize();
	while (true)//ゲームループ
	{
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			break;
		}
		//更新
		Update();
		if (GetFinish()) {
			break;
		}
		//描画
		Draw();
	}
	//終了
	Finalize();
}

void Framework::End()
{

	delete sceneManager_;
}

void Framework::Initialize()
{
	winApp = make_unique<WinApp>();
	winApp->Initialize();
	dxCommon = make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());
	input = Input::GetInstance();
	input->Initialize(winApp.get());
	mouse = Mouse::GetInstance();
	mouse->Initialize(winApp.get());
	camera=make_unique<DebugCamera>(WinApp::window_width, WinApp::window_height);
	

	Object3d::StaticInitialize(dxCommon->GetDev(),camera.get());
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	ParticleManager::StaticInitialize(camera.get(), dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Texture::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height,camera.get());
	Light::StaticInitialize(dxCommon->GetDev());
	LightGroup::StaticInitialize(dxCommon->GetDev());
	spritemanager_ = make_unique<SpriteManager>();
	spritemanager_->SpriteName();
	//シーンマネージャー
	sceneManager_ = new SceneManager;
	//Fbx
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	ModelManager::GetInstance()->Initialize();
}

void Framework::Update()
{

	if (winApp->ProcessMessage()) {
		finish = true;
		return;
	}
	//DirectX毎フレーム処理 ここから
	input->Update();
	mouse->Update();

	//シーンの更新
	sceneManager_->Update(dxCommon.get());
}


void Framework::Finalize()
{

	winApp->Finalize();
	//ウィンドウ表示
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	FbxLoader::GetInstance()->Finalize();


}


void Framework::Draw()
{
	//描画前処理
	//dxCommon->PreDraw();
	//シーン描画
	sceneManager_->Draw(dxCommon.get());

	//描画後処理
	//dxCommon->PostDraw();
}
