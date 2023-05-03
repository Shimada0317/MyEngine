#include "Movie.h"
#include"SpriteManager.h"

Movie::~Movie()
{
	curtaindown_.reset();
	curtainup_.reset();
	skip_.reset();
}

void Movie::Create()
{
	curtainup_.reset(Sprite::SpriteCreate(Name::kCurtain, curtainuppos_));
	curtaindown_.reset(Sprite::SpriteCreate(Name::kCurtain, curtaindownpos_));
	skip_.reset(Sprite::SpriteCreate(Name::kSkip, skippos_));

	curtainup_->SetSize(curtainsize_);
	curtaindown_->SetSize(curtainsize_);
}

void Movie::StatusSet()
{
	curtainup_->SetPosition(curtainuppos_);
	curtaindown_->SetPosition(curtaindownpos_);
	skip_->SetPosition(skippos_);
}

void Movie::Disply()
{
	curtainuppos_.y += 4;
	curtaindownpos_.y -= 4;
	skippos_.y -= 2;

	if (curtainuppos_.y >= 0) {
		curtainuppos_.y = 0;
	}

	if (curtaindownpos_.y <= 620) {
		curtaindownpos_.y = 620;
	}

	if (skippos_.y <= 620) {
		skippos_.y = 620;
	}
}

void Movie::Invisible()
{
	curtainuppos_.y -= 4;
	curtaindownpos_.y += 4;
	skippos_.y += 4;

	if (curtainuppos_.y <= -100) {
		curtainuppos_.y = -100;
	}

	if (curtaindownpos_.y >= 720) {
		curtaindownpos_.y = 720;
	}

	if (skippos_.y >= 720) {
		skippos_.y = 12000;
	}
}

void Movie::Draw()
{
	curtainup_->Draw();
	curtaindown_->Draw();
	skip_->Draw();
}
