#include "MyGame.h"

void MyGame::Initialize()
{
	//Sprite* spriteC = nullptr;
	winApp = new WinApp();
	winApp->Initialize();
	//Sprite* spriteC = nullptr;


	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	//Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	//DirectX初期化処理　ここから
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ID3D12Debug* debugContoroller;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugContoroller))))
	{
		debugContoroller->EnableDebugLayer();
	}
#endif // DEBUG

	HRESULT result;

	//GameScene.hに

	input = new Input();

	gamescene = new GameScene();

	gamescene->StaticInitialize(dxCommon, winApp, input);

	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);

	gamescene->Initialize(dxCommon);
	gamescene->GameSceneInitialize();
}

void MyGame::Update()
{
	if (winApp->ProcessMessage()) {
		finish = true;
		return;
	}
	//DirectX毎フレーム処理 ここから
	input->Update();
	gamescene->Update(dxCommon, input);
}

void MyGame::Draw()
{
	//描画処理
	dxCommon->PreDraw();

	gamescene->Draw(dxCommon);
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	winApp->Finalize();

	//ウィンドウ表示
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	delete input;
	delete winApp;
	delete dxCommon;
	//delete contoroller;
	//delete model;
}
