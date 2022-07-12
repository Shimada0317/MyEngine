#include "Player.h"
#include<cassert>

void Player::Initalize()
{

	input = Input::GetInstance();
	debugtext = DebugText::GetInstance();
	model = ObjModel::LoadFromObJ("block0");
	player = Object3d::Create();
	player->SetModel(model);

	position = player->GetPosition();

	texmodel = TextureModel::loadFromTex(L"Resources/mark.png");
	tex = Texture::Create();
	tex->SetModel(texmodel);
	tex->Update();
};

void Player::Set()
{
	//Eye_pos = { position.x + cameraEye.x,position.y + cameraEye.y,position.z + cameraEye.z };
	//Target_pos = { position.x + cameraTarget.x,position.y + cameraTarget.y,position.z + cameraTarget.z };
	//player->SetPosition(position);

	player->SetEye({ cameraEye });
	player->SetTarget({ cameraTarget });
	player->CameraMoveVector({ camerapos });

	tex->SetScale({ 0.3f,0.3f,0.3f });
	tex->SetPosition({ position });
	tex->SetEye({ cameraEye });
	tex->SetTarget({ cameraTarget });
	tex->CameraMoveEyeVector({ position });


	//tex->SetEye({ cameraEye });
	//tex->SetTarget({ cameraTarget });
	//tex->CameraMoveVector({ position });
}

void Player::Update()
{
	if (stopF == false) {
		Action::GetInstance()->PlayerMove2d(position, 0.03f);
	}

	if (Input::GetInstance()->PushKey(DIK_2)) {
		stopF = true;
	}
	else if(Input::GetInstance()->PushKey(DIK_3)) {
		stopF = false;
	}

	if (stopF == true) {
		Action::GetInstance()->PlayerMove2d(camerapos, 1.3f);

	}
	

	//Action::GetInstance()->PlayerMove2d(cameraTarget, 0.5f);
	Set();
	tex->Update();
	player->Update();
}

void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	Texture::PreDraw(cmdList);
	tex->Draw();
	Texture::PostDraw();
	//player->Draw();
}

void Player::Finalize()
{
	delete tex;
	delete texmodel;
	delete player;
	delete model;
}
