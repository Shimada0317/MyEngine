#include "TitleScene.h"
#include"SceneManager.h"
#include<cassert>
#include <sstream>
#include <iomanip>
#include"GameScene.h"

using namespace DirectX;

TitleScene::TitleScene(SceneManager* sceneManager_)
	:BaseScene(sceneManager_)
{

}

void TitleScene::Initialize(DirectXCommon* dxComon)
{
	////�X�v���C�g�̓ǂݍ���
	Sprite::LoadTexture(1, L"Resources/tst.png");
	title = Sprite::SpriteCreate(1, { 1.0f,1.0f });

	//Sprite::LoadTexture(100, L"Resources/white1x1.png");
	post = new PostEffect();
	post->Initialize();


}

void TitleScene::SetPosSclRot()
{
	title->SetSize({ titlesize });
	title->SetPosition({ titlepos });
}

void TitleScene::Updata()
{
	Action::GetInstance()->PlayerMove2d(titlesize,1.0f);

	//DirectX���t���[������ ��������
	timer += 1;
	if (timer >= 10) {
		if (Input::GetInstance()->PushClick(0)|| Input::GetInstance()->PushClick(1)) {
			//�V�[���؂�ւ�
			BaseScene* scene_ = new GameScene(sceneManager_);
			sceneManager_->SetNextScene(scene_);
			timer = 0;
		}
	}
	SetPosSclRot();
	post->Update(col);
}

void TitleScene::Draw(DirectXCommon* dxCommon)
{

	post->PreDrawScene(dxCommon->GetCmdList());
	Sprite::PreDraw(dxCommon->GetCmdList());
	title->Draw();
	Sprite::PostDraw();
	post->PostDrawScene(dxCommon->GetCmdList());
	
	dxCommon->PreDraw();
	post->Draw(dxCommon->GetCmdList());
	dxCommon->PostDraw();
}

void TitleScene::Finalize()
{
	delete title;
	delete dxCommon;
	delete post;
}