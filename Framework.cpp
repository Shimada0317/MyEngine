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
	winApp = new WinApp();
	winApp->Initialize();
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);
	input = Input::GetInstance();
	input->Initialize(winApp);
	audio = new Audio();
	audio->StaticInitialize();
	audio->LoadFile("Resources/digitalworld.wav", 0.1);
	gamescene = new GameScene();
	gamescene->Initialize(dxCommon);
	
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


void Framework::Finalize()
{

	winApp->Finalize();

	//ウィンドウ表示
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");


	//delete input;
	delete winApp;
	delete dxCommon;
}


void Framework::Draw()
{


}
