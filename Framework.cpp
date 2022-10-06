#include "Framework.h"
#include"FbxLoader.h"


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
	camera = new DebugCamera(WinApp::window_width,WinApp::window_height);
	//audio->LoadFile("Resources/digitalworld.wav", 0.1);
	Object3d::StaticInitialize(dxCommon->GetDev(),camera);
	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	ParticleManager::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Texture::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height,camera);
	Light::StaticInitialize(dxCommon->GetDev());
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);

	//�V�[���}�l�[�W���[
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
	//DirectX���t���[������ ��������
	input->Update();

	//�V�[���̍X�V
	sceneManager_->Update(dxCommon);
}


void Framework::Finalize()
{

	winApp->Finalize();
	
	delete sceneManager_;

	//�E�B���h�E�\��
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	FbxLoader::GetInstance()->Finalize();

	//delete input;
	delete winApp;
	//delete dxCommon;
}


void Framework::Draw()
{



	//�`��O����
	//dxCommon->PreDraw();
	//�V�[���`��
	sceneManager_->Draw(dxCommon);
	//�f�o�b�O�e�L�X�g�`��
	debugText->DrawAll();
	//�`��㏈��
	//dxCommon->PostDraw();
}
