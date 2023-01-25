#include "Action.h"
#include"Mouse.h"
#include<random>

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

void Action::PlayerMove3d(XMVECTOR &position)
{
	XMVECTOR move = { 0.05f,0.05f,0.5f };

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		position.m128_f32[1] += move.m128_f32[1];
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		position.m128_f32[1] -= move.m128_f32[1];
	}
	else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		position.m128_f32[0] += move.m128_f32[0];
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		position.m128_f32[0] -= move.m128_f32[0];
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

float Action::GetRangRand(float minValue, float maxValue)
{
	//シード乱数生成器
	std::random_device rnd;
	//メルセンヌ・ツイスタ方
	std::mt19937_64 mt64(rnd());
	//範囲内の離散分布を作る
	std::uniform_real_distribution<float> genRandFloat(minValue, maxValue);
	//分布の中から生成した乱数を使って1つだけ値を取り出す
	return genRandFloat(mt64);
}

void Action::EaseOut(float& MoveValue, float PurposeValue)
{
	float dx = (PurposeValue - MoveValue) / 50.0f;
	MoveValue += dx;
}





