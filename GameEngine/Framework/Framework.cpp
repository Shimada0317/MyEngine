#include "Framework.h"
#include"FbxLoader.h"
#include"ModelManager.h"


void Framework::Run()
{
	//������
	Initialize();
	while (true)//�Q�[�����[�v
	{
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			break;
		}
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
	//�V�[���}�l�[�W���[
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
	//DirectX���t���[������ ��������
	input->Update();
	mouse->Update();

	//�V�[���̍X�V
	sceneManager_->Update(dxCommon.get());
}


void Framework::Finalize()
{

	winApp->Finalize();
	//�E�B���h�E�\��
	ShowWindow(winApp->GetHwnd(), SW_SHOW);
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	FbxLoader::GetInstance()->Finalize();


}


void Framework::Draw()
{
	//�`��O����
	//dxCommon->PreDraw();
	//�V�[���`��
	sceneManager_->Draw(dxCommon.get());

	//�`��㏈��
	//dxCommon->PostDraw();
}
