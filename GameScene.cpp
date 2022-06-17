#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"SceneManager.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"

using namespace DirectX;

GameScene::GameScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void GameScene::Initialize(DirectXCommon* dxComon)
{
	
	Sprite::LoadTexture(100, L"Resources/white1x1.png");
	postEffect = new PostEffect();
	postEffect->Initialize();

	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);

	
	////�X�v���C�g�̓ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background.png");
	
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	
	//postEffect->InitializeSprite();

	//���f���̓ǂݍ���
	/*playermodel = Model::LoadFromObJ("player");
	player3d = Object3d::Create();
	player3d->SetModel(playermodel);*/

	ramieru = Model::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);

	
	
	

	//���f�������w�肵�ăt�@�C���ǂݍ���
	model = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	FbxObject3d::SetDevice(dxComon->GetDev());
	FbxObject3d::SetCamera(camera);
	FbxObject3d::CreateGraphicPipeline();

	Object = new FbxObject3d;
	Object->Initialize();
	Object->SetModel(model);

}

void GameScene::SetPosSclRot()
{
	//�v���C���[
	/*player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});*/


	camera->SetEye({ 0,0,90 });
	camera->SetTarget({ 0,2.5f,0 });
	camera->SetDistance({ 8.0f });

	ramieru3d->SetRotation({ 0,0,0 });
	ramieru3d->SetPosition({ ramieru_pos });
	ramieru3d->SetScale({ 1.0f,1.0f,1.0f });

	Object->SetRotation({ 0.0f,90.0f,0.0f });

	title->SetSize({ 1280.0f,720.0f });
}

void GameScene::Update()
{
	//DirectX���t���[������ ��������
	

	if (Input::GetInstance()->PushKey(DIK_1)) {
		Object->PlayAnimation();
	}
	
	SetPosSclRot();
	camera->Update();
	ramieru3d->Update();
	
	Object->Update();
	

}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{
	//�I�u�W�F�N�g�O����
	Object3d::PreDraw();
	//player3d->Draw();
	//ramieru3d->Draw();
	//human3d->Draw();
	//�I�u�W�F�N�g�㏈��
	Object3d::PostDraw();
	Object->Draw(dxCommon->GetCmdList());
}

void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	//title->Draw();
	Sprite::PostDraw();
}



void GameScene::Draw(DirectXCommon* dxCommon)
{
	postEffect->PreDrawScene(dxCommon->GetCmdList());
	SpriteDraw(dxCommon);
	ObjDraw(dxCommon);
	postEffect->PostDrawScene(dxCommon->GetCmdList());
	//�`��O����
	dxCommon->PreDraw();
	postEffect->Draw(dxCommon->GetCmdList());
	//�`��㏈��
	dxCommon->PostDraw();
}

void GameScene::Finalize()
{
	delete title;
	delete player3d;
	delete winApp;
	delete dxCommon;
}