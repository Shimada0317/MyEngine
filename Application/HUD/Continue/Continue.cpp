#include "Continue.h"
#include"SpriteManager.h"

Continue::~Continue()
{
	continue_.reset();
	yes_.reset();
	no_.reset();
}

void Continue::Create(const XMFLOAT2& yesposition_, const XMFLOAT2& noposition_)
{
	XMFLOAT2 anchorpoint_{ 0.5f,0.5f };

	continue_.reset(Sprite::SpriteCreate(kGameOver, { 0.f,0.f }));
	yes_.reset(Sprite::SpriteCreate(kYes, yesposition_, yescolor_, anchorpoint_));
	no_.reset(Sprite::SpriteCreate(kNo, noposition_, nocolor_, anchorpoint_));
}

void Continue::ChangeColor(const XMFLOAT4& yescolor_, const XMFLOAT4& nocolor_)
{
	yes_->SetColor(yescolor_);
	no_->SetColor(nocolor_);
}

void Continue::Draw()
{
	continue_->Draw();
	yes_->Draw();
	no_->Draw();
}
