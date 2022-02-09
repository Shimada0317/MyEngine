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

	//DirectX�����������@��������
#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ID3D12Debug* debugContoroller;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugContoroller))))
	{
		debugContoroller->EnableDebugLayer();
	}
#endif // DEBUG

	HRESULT result;

	//GameScene.h��

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
	//DirectX���t���[������ ��������
	input->Update();
	gamescene->Update(dxCommon, input);
}

void MyGame::Draw()
{
	//�`�揈��
	dxCommon->PreDraw();

	gamescene->Draw(dxCommon);
	dxCommon->PostDraw();
}

void MyGame::Finalize()
{
	winApp->Finalize();

	//�E�B���h�E�\��
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	delete input;
	delete winApp;
	delete dxCommon;
	//delete contoroller;
	//delete model;
}
