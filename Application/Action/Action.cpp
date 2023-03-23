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
	XMVECTOR move = { 0.5f,0.5f,0.5f };

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
	float easevalue = (PurposeValue - MoveValue) / 30.0f;
	MoveValue += easevalue;
}

float Action::EasingOut(float time,float addvalue)
{
	return addvalue * sinf((time * DirectX::XM_PI) / 2);
}

void Action::Flash(float& color, float transparent)
{
	color -= transparent;
}

void Action::ThrowUp(float gravity, float time, float upper, float& position)
{
	position += gravity * time - upper;
}

DirectX::XMFLOAT3 Action::ConvertToXMFLOAT3(float SourceValue)
{
	XMFLOAT3 ConvertValue;
	
	ConvertValue.x = SourceValue;
	ConvertValue.y = SourceValue;
	ConvertValue.z = SourceValue;

	return ConvertValue;
}

bool Action::CompletelyTogetherXMVECTOR(const XMVECTOR& FirstValue, const XMVECTOR& SecondValue)
{
	if (FirstValue.m128_f32[0] == SecondValue.m128_f32[0] &&
		FirstValue.m128_f32[1] == SecondValue.m128_f32[1] &&
		FirstValue.m128_f32[2] == SecondValue.m128_f32[2]) {
		return true;
	}

	return false;
}

void Action::DebugMove(XMFLOAT3& Value)
{
	float movevalue = 0.1f;
	if (Input::GetInstance()->PushKey(DIK_W)) {
		Value.y += movevalue;
	}

	if (Input::GetInstance()->PushKey(DIK_S)) {
		Value.y -= movevalue;
	}

	if (Input::GetInstance()->PushKey(DIK_A)) {
		Value.x -= movevalue;
	}

	if (Input::GetInstance()->PushKey(DIK_D)) {
		Value.x += movevalue;
	}

	if (Input::GetInstance()->PushKey(DIK_Q)) {
		Value.z -= movevalue;
	}

	if (Input::GetInstance()->PushKey(DIK_E)) {
		Value.z += movevalue;
	}
}









