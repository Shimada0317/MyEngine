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

void Action::PlayerMove3d(XMVECTOR &position, float Speed)
{
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		position.m128_f32[1] += Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		position.m128_f32[1] -= Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		position.m128_f32[0] += Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		position.m128_f32[0] -= Speed;
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

void Action::PlayerMove2d(XMFLOAT2 &position, float Speed)
{
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		position.y += Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		position.y -= Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		position.x += Speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		position.x -= Speed;
	}
}

void Action::Gunshot(int Remainigbullet, bool& shot)
{
 	if (Input::GetInstance()->PushClick(0)||Input::GetInstance()->PushKey(DIK_SPACE)) {
		shot = true;
	}

}

bool Action::Shot(int bullet, bool& shot)
{
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		if (bullet > 0) {
			shot = true;
			return true;
		}
		else {
			return false;
		}
	}

	
}



