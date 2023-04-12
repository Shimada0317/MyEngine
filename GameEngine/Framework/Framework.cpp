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
	sceneManager_->Update(dxCommon);
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
	sceneManager_->Draw(dxCommon);
	//�f�o�b�O�e�L�X�g�`��
	debugText->DrawAll();
	//�`��㏈��
	//dxCommon->PostDraw();
}
