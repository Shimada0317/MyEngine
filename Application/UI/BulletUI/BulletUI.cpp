#include"Action.h"
#include "BulletUI.h"
#include"SpriteManager.h"
//�e��
const int MaxRemainingBullet = 9;

BulletUI::~BulletUI()
{
	reload_ui_.reset();
	for (int i = {}; i < MaxRemainingBullet; i++) {
		bullet_ui_[i].reset();
	}
}

void BulletUI::Create(int remainig, const XMFLOAT2& bulletuipos, const XMFLOAT2& reloadpos)
{
	const XMFLOAT4 Color_{ 1.f,1.f,1.f,1.f };
	//Sprite�̃A���J�[�|�C���g
	XMFLOAT2 Anchorpoint_{ 0.5f,0.5f };
	//HUD�̃T�C�Y
	const XMFLOAT2 SpriteSiz = { 64.0f,64.0f };
	remaining_ = old_remaining_ = remainig;
	original_bulletpos_ = bulletuipos;
	//�X�v���C�g�̐���
	reload_ui_.reset(Sprite::SpriteCreate(Name::kReload, reloadpos, reload_color_, Anchorpoint_));
	for (int i = {}; i < MaxRemainingBullet; i++) {
		bullet_pos_[i] = { original_bulletpos_.x,original_bulletpos_.y + 32.0f * i };
		bullet_rot_[i] = {};
		time_[i] = {};
		bullet_ui_[i].reset(Sprite::SpriteCreate(Name::kBullet, bullet_pos_[i], Color_, Anchorpoint_));
		bullet_ui_[i]->SetSize(SpriteSiz);
		drop_bulletflag_[i] = false;
	}
}

void BulletUI::Set()
{
	//UI�̍��W�Ɖ�]�̃Z�b�g
	for (int i = {}; i < MaxRemainingBullet; i++) {
		bullet_ui_[i]->SetPosition(bullet_pos_[i]);
		bullet_ui_[i]->SetRotation(bullet_rot_[i]);
	}
	//reload�̕����̃T�C�Y�̃Z�b�g
	reload_ui_->SetSize(reload_size_);
	reload_ui_->SetColor(reload_color_);
}

void BulletUI::Shot(int remaining)
{
	remaining_ = remaining;
	if (old_remaining_ < remaining_) {
		drop_bulletflag_[old_remaining_] = true;
		old_remaining_ = remaining_;
	}
}

void BulletUI::Reload(int remaining)
{
	remaining_ = remaining;
	if (remaining_ == 0) {
		old_remaining_ = remaining_;
		for (int i = {}; i < MaxRemainingBullet; i++) {
			bullet_pos_[i] = { original_bulletpos_.x,original_bulletpos_.y + 32.f * i };
			bullet_rot_[i] = {};
			time_[i] = {};
			drop_bulletflag_[i] = false;
		}
	}
}

void BulletUI::FallingUI()
{
	//�������ɉ�]�ɉ��Z����l
	const float addrotationvalue_ = 80.f;
	//���E�ɔ�΂��l
	const float absolutevalue_ = 10.f;
	//���Ԃɉ��Z����l
	const float addfalltime_ = 0.5f;
	//�d��
	const float gravity_ = 9.8f;
	//������ɔ�΂��l
	const int upper_ = 40;
	//�e��9���Ԃ��for��
	for (int i = {}; i < MaxRemainingBullet; i++) {
		if (drop_bulletflag_[i]) {
			time_[i] += addfalltime_;
			bullet_pos_[i].x += Action::GetInstance()->GetRangRand(-absolutevalue_, absolutevalue_);
			Action::GetInstance()->ThrowUp(gravity_, time_[i], upper_, bullet_pos_[i].y);
			bullet_rot_[i] += addrotationvalue_;
		}
	}
}

void BulletUI::ReloadMotion()
{
	float absolutevalue = 0.05f;
	float absolutelimit = 1.f;
	if (easing_changeflag_) {
		reload_time_ += absolutevalue;
		if (reload_time_ >= absolutelimit) {
			easing_changeflag_ = false;
		}
	}
	else {
		reload_time_ -= absolutevalue;
		if (reload_time_ <= -absolutelimit) {
			easing_changeflag_ = true;
		}
	}

	reload_size_.x = Action::GetInstance()->EasingOut(reload_time_, 20) + reload_oldsize_.x;
	reload_size_.y = Action::GetInstance()->EasingOut(reload_time_, 20) + reload_oldsize_.y;
}

void BulletUI::Draw()
{
	for (int i = {}; i < MaxRemainingBullet; i++) {
		if (remaining_ <= MaxRemainingBullet) {
			bullet_ui_[i]->Draw();
		}
	}

	if (remaining_ >= MaxRemainingBullet) {
		reload_ui_->Draw();
	}
}
