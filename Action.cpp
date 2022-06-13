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
