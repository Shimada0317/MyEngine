#include "Framework.h"

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
	input->Initialize(winApp);

	gamescene = new GameScene();
	audio = new Audio();
	audio->StaticInitialize();
	

	gamescene->Initialize(dxCommon);
	audio->LoadFile("Resources/digitalworld.wav", 0.1);
}

void Framework::Finalize()
{

	winApp->Finalize();

	//ウィンドウ表示
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");


	delete input;
	delete winApp;
	delete dxCommon;
}

void Framework::Update()
{
	
	if (winApp->ProcessMessage()) {
		finish = true;
		return;
	}
	//DirectX毎フレーム処理 ここから
	input->Update();
}

void Framework::Draw()
{


}
