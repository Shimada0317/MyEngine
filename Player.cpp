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
	
};

void Player::Set()
{
	//Eye_pos = { position.x + cameraEye.x,position.y + cameraEye.y,position.z + cameraEye.z };
	//Target_pos = { position.x + cameraTarget.x,position.y + cameraTarget.y,position.z + cameraTarget.z };
	player->SetPosition(position);
	player->SetEye({ cameraEye});
	player->SetTarget({ cameraTarget });
	player->CameraMoveVector({ position });
}

void Player::Update()
{
	Action::GetInstance()->PlayerMove2d(position, 0.5f);
	//Action::GetInstance()->PlayerMove2d(cameraTarget, 0.5f);
	player->Update();

}

void Player::Draw()
{
	player->Draw();
}
