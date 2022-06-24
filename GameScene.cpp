#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"Action.h"
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
	
	//Sprite::LoadTexture(100, L"Resources/background.png");
	postEffect = new PostEffect();
	postEffect->Initialize();

	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	Object3d::SetCamera(camera);
	
	////�X�v���C�g�̓ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background.png");
	
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	
	//postEffect->InitializeSprite();

	//���f���̓ǂݍ���
	playermodel = ObjModel::LoadFromObJ("skydome");
	sphere = Object3d::Create();
	sphere->SetModel(playermodel);

	ramieru = ObjModel::LoadFromObJ("ramieru");
	ramieru3d = Object3d::Create();
	ramieru3d->SetModel(ramieru);

	ground = ObjModel::LoadFromObJ("ground");
	groundObj = Object3d::Create();
	groundObj->SetModel(ground);
	
	

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
	sphere->SetRotation({ 0,0,0 });
	sphere->SetPosition({ pos });
	sphere->SetScale({4.0f,4.0f,4.0f});


	camera->SetDistance({ cameradistance });
	camera->SetEye({ cameraEye });
	camera->SetTarget({ cameraTarget });

	ramieru3d->SetRotation({ ramieru_rot });
	ramieru3d->SetPosition({ ramieru_pos });
	ramieru3d->SetScale({ ramieru_scl });

	groundObj->SetPosition({ ground_pos });
	groundObj->SetScale({ ground_scl });

	Object->SetRotation({ obj_rot });
	Object->SetPosition({ obj_pos });

	title->SetSize({ screen_size });
}

void GameScene::Update()
{
	//DirectX���t���[������ ��������
	

	if (Input::GetInstance()->PushKey(DIK_1)) {
		Object->PlayAnimation();
	}

	//Action::GetInstance()->PlayerMove3d(cameraEye, 0.5f);
	Action::GetInstance()->PlayerMove3d(obj_pos, 0.02f);
	Action::GetInstance()->PlayerJump(ramieru_pos,JumpFlag);

	SetPosSclRot();
	camera->Update();
	ramieru3d->Update();
	sphere->Update();
	Object->Update();
	groundObj->Update();
	

}

void GameScene::ObjDraw(DirectXCommon* dxCommon)
{
	//�I�u�W�F�N�g�O����
	Object3d::PreDraw();
	//sphere->Draw();
	groundObj->Draw();
	ramieru3d->Draw();
	//human3d->Draw();
	//�I�u�W�F�N�g�㏈��
	Object3d::PostDraw();
	Object->Draw(dxCommon->GetCmdList());
}

void GameScene::SpriteDraw(DirectXCommon* dxCommon)
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();
}



void GameScene::Draw(DirectXCommon* dxCommon)
{
	postEffect->PreDrawScene(dxCommon->GetCmdList());
	//SpriteDraw(dxCommon);
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
	delete sphere;
	delete groundObj;
	delete ramieru3d;
	//delete sphere;
	delete winApp;
	delete dxCommon;
}