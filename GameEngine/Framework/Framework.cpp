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
	delete dxCommon;
	delete winApp;
	delete sceneManager_;
}

void Framework::Initialize()
{
	winApp = new WinApp();
	winApp->Initialize();
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	input = Input::GetInstance();
	input->Initialize(winApp);
	mouse = Mouse::GetInstance();
	mouse->Initialize(winApp);
	camera = new DebugCamera(WinApp::window_width,WinApp::window_height);
	

	Object3d::StaticInitialize(dxCommon->GetDev(),camera);
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	ParticleManager::StaticInitialize(camera,dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Texture::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height,camera);
	Light::StaticInitialize(dxCommon->GetDev());
	LightGroup::StaticInitialize(dxCommon->GetDev());
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);
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
	sceneManager_->Update(dxCommon);
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
	sceneManager_->Draw(dxCommon);
	//デバッグテキスト描画
	debugText->DrawAll();
	//描画後処理
	//dxCommon->PostDraw();
}
