#pragma once
#include"Sprite.h"
#include"HelperMath.h"

class PlayerUI
{
public:
	~PlayerUI();

	void Create();

	void HartBeat(int playerhp_);

	void Draw(int playerhp_);
private:
	unique_ptr<Sprite> life_[5];
	unique_ptr<Sprite> hart_;

	XMFLOAT2 hart_size_{ 64.f,64.f };
	XMFLOAT2 original_size_{ 0.f,0.f };
	float easingtimer_ = 0.f;
	float add_timer_ = 0.01f;
	bool reverse_flag_ = true;

};

