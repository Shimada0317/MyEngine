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
	//DirectX���t���[������ ��������
	input->Update();
}


void Framework::Finalize()
{

	winApp->Finalize();

	//�E�B���h�E�\��
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");


	//delete input;
	delete winApp;
	delete dxCommon;
}


void Framework::Draw()
{


}
