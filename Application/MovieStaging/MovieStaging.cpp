#include "MovieStaging.h"
#include"Action.h"
#include"Mouse.h"
#include"Player.h"

void MovieStaging::StartMovie(Player* player, XMFLOAT3& eyerot, XMVECTOR& velocity, RailCamera* railcamera)
{
	XMVECTOR PlayerPos =player->GetPosition();
	XMVECTOR PlayerWorldPos = player->GetBodyWorldPos();
	//後ろを向く
	if (moviesequence_ == MovieSequence::kTurnAround) {
		Action::GetInstance()->EaseOut(eyerot.y, -5.0f);
		if (eyerot.y <= 0) {
			eyerot.y = 0;
			moviesequence_ = MovieSequence::kFaceLower;
		}
	}
	//下を向く
	if (moviesequence_ == MovieSequence::kFaceLower) {
		actiontimer_ += 0.2f;
		if (actiontimer_ > 5) {
			actiontimer_ = 5.0f;
			Action::GetInstance()->EaseOut(eyerot.x, 95.0f);
			if (eyerot.x >= 90) {
				actiontimer_ = 0.0f;
				eyerot.x = 90;
				moviesequence_ = MovieSequence::kJump;
			}
		}
	}
	//跳ぶ
	if (moviesequence_ == MovieSequence::kJump) {
		actiontimer_ += 0.15f;
		velocity = { 0.0f,0.67f,0.4f };
		if (actiontimer_ >= 5) {
			velocity = { 0.0f,-0.6f,0.0f };
			Action::GetInstance()->EaseOut(eyerot.x, -5.0f);
		}
		if (eyerot.x <= 0.0f) {
			eyerot.x = 0.0f;

		}
		if (PlayerWorldPos.m128_f32[1] <= 0.9f) {
			PlayerWorldPos.m128_f32[1] = 0.9f;
			velocity = { 0.0f,0.0f,0.0f };
			moviesequence_ = MovieSequence::kLanding;
		}
	}
	//地面に着いたとき
	if (moviesequence_ == MovieSequence::kLanding) {
		PlayerPos = { 0.0f,-0.7f,13.0f };
		railcamera->MatrixIdentity(PlayerPos, eyerot);
		moviesequence_ = MovieSequence::kFinish;
	}
	//演出スキップ
	SkipStartMovie(PlayerPos,PlayerWorldPos,eyerot,velocity,railcamera);

	player->SetBodyWorldPos(PlayerWorldPos);
}

void MovieStaging::SkipStartMovie(XMVECTOR& PlayerPos, XMVECTOR& PlauerWorldPos, XMFLOAT3& eyerot, XMVECTOR& velocity, RailCamera* railcamera)
{
	if ((Mouse::GetInstance()->PushClick(1) || Mouse::GetInstance()->PushClick(0))) {
		eyerot.x = 0;
		eyerot.y = 0;
		velocity = { 0.0f,0.0f,0.0f };
		PlayerPos = { 0.0f,-0.7f,13.0f };
		railcamera->MatrixIdentity(PlayerPos, eyerot);
		PlauerWorldPos.m128_f32[1] = 0.9f;
		moviesequence_ = MovieSequence::kFinish;
	}
}

void MovieStaging::MoveShakingHead(XMFLOAT3& eyerot)
{
	if (moviesequence_ != MovieSequence::kFinish) { return; }
	Action::GetInstance()->MoveShakingHead(eyerot);
}
