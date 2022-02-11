#include "Framework.h"

void Framework::Run()
{
	//������
	Initialize();
	while (true)//�Q�[�����[�v
	{
		//�X�V
		Update();
		if (GetFinish()) {
			break;
		}
		//�`��
		Draw();
	}
	//�I��
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

	//�E�B���h�E�\��
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//�R���\�[���ւ̕����o��
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
	//DirectX���t���[������ ��������
	input->Update();
}

void Framework::Draw()
{


}
