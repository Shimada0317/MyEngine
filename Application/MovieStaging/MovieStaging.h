#pragma once
#include"HelperMath.h"
#include"RailCamera.h"

class Player;

enum MovieSequence {
	kAction = 0,
	kTurnAround,
	kFaceLower,
	kJump,
	kLanding,
	kFinish,
};

class MovieStaging
{
public:
	void StartMovie(Player* player,XMFLOAT3& eyerot,XMVECTOR& velocity,RailCamera* railcamera);
	void SkipStartMovie(XMVECTOR& reticlepos, XMVECTOR& bodyworldpos, XMFLOAT3& eyerot, XMVECTOR& velocity, RailCamera* railcamera);
	void MoveShakingHead(XMFLOAT3& eyerot);
	int GetMovieSequence() { return moviesequence_; }
private:
	float actiontimer_ = 0.f;
	int moviesequence_ = MovieSequence::kTurnAround;

};

