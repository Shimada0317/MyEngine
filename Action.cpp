#include "Action.h"

Action::Action()
{
}

Action::~Action()
{
}

Action* Action::GetInstance()
{
	static Action instance;

	return &instance;
}

void Action::PlayerMove3d(XMFLOAT3 &position, float Speed)
{
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		position.z += Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		position.z -= Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		position.x += Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		position.x -= Speed;
	}
}

void Action::PlayerJump(XMFLOAT3& position,bool& JumpFlag)
{

	if (JumpFlag == false) {
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			JumpFlag = true;
		}
	}
	else if (JumpFlag == true) {
		position.y += 0.01f;
	}

}

void Action::PlayerMove2d(XMFLOAT2 position, float Speed)
{
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		position.y += Speed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		position.y -= Speed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		position.x += Speed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		position.x -= Speed;
	}
}

void Action::Gunshot(bool alive, bool havegun, int Remainigbullet, bool shot)
{
	if (alive != true)return;
	if (havegun != true)return;
	if (Remainigbullet < 0)return;
	shot = true;
}



