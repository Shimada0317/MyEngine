#include "GameScene.h"
#include<cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

void GameScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);

	this->dxCommon = dxCommon;

	Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	Object3d::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height);
	////�X�v���C�g�̓ǂݍ���

	Sprite::LoadTexture(1, L"Resources/title.png");


	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);

	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });


	//���f���̓ǂݍ���
	playermodel = Model::LoadFromObJ("player");

	//�I�u�W�F�N�g�̃N���G�C�g
	player3d = Object3d::Create();

	//���f���̃Z�b�g
	player3d->SetModel(playermodel);

}


void GameScene::GameSceneInitialize()
{
	//�v���C���[
	player3d->SetRotation({ 0,450,-270 });
	player3d->SetPosition({ 0,0,0 });
	player3d->SetScale({0.5f,0.5f,0.5f});

	title->SetSize({ 1280.0f,720.0f });
}

void GameScene::Update(DirectXCommon* dxCommon,Input* input)
{
	//DirectX���t���[������ ��������
	
	this->dxCommon = dxCommon;
	//this->audio = audio;

	//audio->LoadFile("Resources/digitalworld.wav", vol[0]);

	player3d->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	// �R�}���h���X�g�̎擾
	//ID3D12GraphicsCommandList* cmdList = dxCommon->GetCmdList();

	//dxCommon->GetCmdList()->IASetVertexBuffers(0, 1, &vbView);

	dxCommon->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	debugText->DrawAll(dxCommon->GetCmdList());
	Sprite::PostDraw();

	//�I�u�W�F�N�g�O����
	Object3d::PreDraw(dxCommon->GetCmdList());
	player3d->Draw();
	//�I�u�W�F�N�g�㏈��
	Object3d::PostDraw();

}

void GameScene::Finalize()
{
	delete title;
	delete player3d;
}