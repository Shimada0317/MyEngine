#include "Framework.h"
#include"FbxLoader.h"


void Framework::Run()
{
	//初期化
	Initialize();
	while (true)//ゲームループ
	{
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

void Framework::Initialize()
{
	winApp = new WinApp();
	winApp->Initialize();
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	input = Input::GetInstance();
	input->Initialize(winApp);
	audio = new Audio();
	audio->StaticInitialize();
	camera = new DebugCamera(WinApp::window_width,WinApp::window_height);
	//audio->LoadFile("Resources/digitalworld.wav", 0.1);
	Object3d::StaticInitialize(dxCommon->GetDev(),camera);
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	ParticleManager::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Texture::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height,camera);
	Light::StaticInitialize(dxCommon->GetDev());
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);

	//シーンマネージャー
	sceneManager_ = new SceneManager;
	

	//Fbx
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

}

void Framework::Update()
{

	if (winApp->ProcessMessage()) {
		finish = true;
		return;
	}
	//DirectX毎フレーム処理 ここから
	input->Update();

	//シーンの更新
	sceneManager_->Update(dxCommon);
}


void Framework::Finalize()
{

	winApp->Finalize();
	
	delete sceneManager_;

	//ウィンドウ表示
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	FbxLoader::GetInstance()->Finalize();

	//delete input;
	delete winApp;
	//delete dxCommon;
}


void Framework::Draw()
{



	//描画前処理
	//dxCommon->PreDraw();
	//シーン描画
	sceneManager_->Draw(dxCommon);
	//デバッグテキスト描画
	debugText->DrawAll();
	//描画後処理
	//dxCommon->PostDraw();
}
