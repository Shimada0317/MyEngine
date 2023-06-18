#include "Action.h"
#include"Camera.h"
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

void Action::PlayerMove3d(XMVECTOR& position)
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

void Action::PlayerJump(XMFLOAT3& position, bool& JumpFlag)
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

void Action::PlayerMove2d(XMFLOAT2& position, float Speed)
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

void Action::EasingAddTimer(float& timer_, float addtime_)
{
	timer_ += addtime_;
	if (timer_ == 1) {
		timer_ = 0;
	}
}

float Action::EasingOut(float time, float addvalue)
{
	return addvalue * sinf((time * DirectX::XM_PI) / 2);
}

XMFLOAT3 Action::EasingOutXMFLOAT3(float time, const XMFLOAT3& addvalue)
{
	XMFLOAT3 valueresult;

	valueresult.x = EasingOut(time, addvalue.x);
	valueresult.y = EasingOut(time, addvalue.x);
	valueresult.z = EasingOut(time, addvalue.x);

	return valueresult;
}

void Action::Flash(float& color, float transparent)
{
	color -= transparent;
}

void Action::ThrowUp(float gravity, float time, float upper, float& position)
{
	position += gravity * time - upper;
}

void Action::ScreenShake(float& shakevalue, float shakingtime, XMFLOAT3& eyerot_, bool& shakingstartflag_)
{
	if (shakingstartflag_ == true) {
		if (shakelimittime_ <= 1) {
			shakelimittime_ += shakingtime;
			if (shakingscreenflag_ == true) {
				shakingscreenvalue_ -= shakevalue;
				if (shakingscreenvalue_ <= -shakevalue) {
					shakingscreenflag_ = false;
				}
			}
			else {
				shakingscreenvalue_ += shakevalue;
				if (shakingscreenvalue_ >= shakevalue) {
					shakingscreenflag_ = true;
				}
			}
			eyerot_.x += shakingscreenvalue_;
		}
		else {
			shakingscreenflag_ = true;
			shakelimittime_ = 0;
			shakingstartflag_ = false;
			shakevalue = 0.f;
			shakingscreenvalue_ = 0;
			eyerot_.x = 0;
		}
	}
}

void Action::MoveShakingHead(XMFLOAT3& eyerot_)
{
	//加算と減算する為の絶対値
	const float EyeRotAbsouluteValue = 0.05f;
	//反転させるための絶対値
	const float AbsoluteValue = 0.5f;
	if (shake_ == true) {
		eyerot_.x += EyeRotAbsouluteValue;
		if (eyerot_.x >= AbsoluteValue) {
			shake_ = false;
		}
	}
	else {
		eyerot_.x -= EyeRotAbsouluteValue;
		if (eyerot_.x <= -AbsoluteValue) {
			shake_ = true;
		}
	}
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

void Action::LoopTimer(float& timer, float addvalue, float duration_)
{
	if (easing_invert_ == false) {
		timer += addvalue;
		if (timer > duration_) {
			easing_invert_ = true;
		}
	}
	else {
		timer -= addvalue;
		if (timer < -duration_) {
			easing_invert_ = false;
		}
	}
}

void Action::ColorDown(XMFLOAT4& color, float downval)
{
	color.x -= downval;
	color.y -= downval;
	color.z -= downval;
	color.w -= downval;
}

void Action::ColorUp(XMFLOAT4& color, float upval)
{
	color.x += upval;
	color.y += upval;
	color.z += upval;
	color.w += upval;
}

XMFLOAT3 Action::RandMax(const XMFLOAT3& value, float randvel)
{
	XMFLOAT3 Value = value;

	Value.x = (float)rand() / RAND_MAX * randvel - randvel / 2.0f;
	Value.y = (float)rand() / RAND_MAX * randvel - randvel / 2.0f;
	Value.z = (float)rand() / RAND_MAX * randvel - randvel / 2.0f;

	return Value;
}

XMFLOAT3 Action::RandMinAndMax(const XMFLOAT3& minvalue, const XMFLOAT3& maxvalue)
{
	XMFLOAT3 Value;

	Value.x = GetRangRand(minvalue.x, maxvalue.x);
	Value.y = GetRangRand(minvalue.y, maxvalue.y);
	Value.z = GetRangRand(minvalue.z, maxvalue.z);

	return Value;
}

XMFLOAT2 Action::WorldToScreen(const XMMATRIX& centermat, const XMVECTOR& position, Camera* camera)
{
	const float kDistancePlayerTo3DReticle = 50.0f;
	XMVECTOR offset_ = { 0.0,0.0,1.0f };
	offset_ = XMVector3TransformNormal(offset_, centermat);
	offset_ = XMVector3Normalize(offset_) * kDistancePlayerTo3DReticle;

	XMVECTOR PositionRet = position;
	XMMATRIX matviewport;
	HelperMath::GetInstance()->ChangeViewPort(matviewport, offset_);

	XMMATRIX MatVP = matviewport;

	XMMATRIX View = camera->GetViewMatrix();
	XMMATRIX Pro = camera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}










