#include "PlayerUI.h"
#include"SpriteManager.h"
#include"WinApp.h"
#include"Action.h"

PlayerUI::~PlayerUI()
{
	hart_.reset();
	for (int i = 0; i < 5; i++) {
		life_[i].reset();
	}
}

void PlayerUI::Create()
{
	const XMFLOAT2 anchorpoint_{ 0.5f,0.5f };
	const XMFLOAT2 hart_position_{ WinApp::window_width - 173,WinApp::window_height - 50 };
	const XMFLOAT2 life_position_{ 1160.f,630.f };
	const XMFLOAT2 life_size_{ 80.f,80.f };
	const XMFLOAT4 color_{ 1.f,1.f,1.f,1.f };
	original_size_ = hart_size_;
	hart_.reset(Sprite::SpriteCreate(kHart, hart_position_, color_, anchorpoint_));
	hart_->SetSize(hart_size_);
	for (int i = 0; i < 5; i++) {
		life_[i].reset(Sprite::SpriteCreate(i, life_position_));
		life_[i]->SetSize(life_size_);
	}

}

void PlayerUI::HartBeat(int playerhp_)
{
	//タイマーの加算
	easingtimer_ += add_timer_;
	//反転フラグがtrueの時サイズを拡大
	if (reverse_flag_) {
		hart_size_.x = Action::GetInstance()->EasingOut(easingtimer_, 40) + original_size_.x;
		hart_size_.y = Action::GetInstance()->EasingOut(easingtimer_, 40) + original_size_.y;
		if (easingtimer_ >= 1) {
			easingtimer_ = 0;
			original_size_ = hart_size_;
			reverse_flag_ = false;
		}
	}
	//反転フラグがfalseの時サイズを縮小
	else {
		hart_size_.x = -Action::GetInstance()->EasingOut(easingtimer_, 40) + original_size_.x;
		hart_size_.y = -Action::GetInstance()->EasingOut(easingtimer_, 40) + original_size_.y;
		if (easingtimer_ >= 1) {
			easingtimer_ = 0;
			original_size_ = hart_size_;
			reverse_flag_ = true;
		}
	}


	if (playerhp_ >= 4) {
		add_timer_ = 0.01f;
	}
	else if (playerhp_ == 3) {
		add_timer_ = 0.05f;
	}
	else if (playerhp_ == 2) {
		add_timer_ = 0.1f;
	}
	else if (playerhp_ == 1) {
		add_timer_ = 0.5f;
	}

	hart_->SetSize(hart_size_);
}

void PlayerUI::Draw(int playerhp_)
{
	hart_->Draw();

	if (playerhp_ == 1) {
		life_[0]->Draw();
	}
	else if (playerhp_ == 2) {
		life_[1]->Draw();
	}
	else if (playerhp_ == 3) {
		life_[2]->Draw();
	}
	else if (playerhp_ == 4) {
		life_[3]->Draw();
	}
	else if (playerhp_ == 5) {
		life_[4]->Draw();
	}
}
