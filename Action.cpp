#include "Action.h"

Action* Action::GetInstance()
{
	static Action instance;

	return &instance;
}

void Action::PlayerMove3d(XMFLOAT3 position, XMFLOAT3 Speed)
{
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		position.y += Speed.y;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		position.y -= Speed.y;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		position.x += Speed.x;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		position.x -= Speed.x;
	}
}

void Action::PlayerMove2d(XMFLOAT2 position, XMFLOAT2 Speed)
{
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		position.y += Speed.y;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		position.y -= Speed.y;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		position.x += Speed.x;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		position.x -= Speed.x;
	}
}

void Action::Gunshot(bool alive, bool havegun, int Remainigbullet, bool shot)
{
	if (alive != true)return;
	if (havegun != true)return;
	if (Remainigbullet < 0)return;
	shot = true;
}



