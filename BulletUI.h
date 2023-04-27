#pragma once
#include"HelperMath.h"
#include"Sprite.h"
#include"WinApp.h"

class BulletUI
{
public:
	~BulletUI();

	void Create(int remaining,const XMFLOAT2& bulletuipos,const XMFLOAT2& reloadpos);

	void Set();

	void Shot(int remaining);

	void Reload(int  remaining);

	void FallingUI();

	void ReloadMotion();

	void Draw();
private:
	//Sprite
	unique_ptr<Sprite> bullet_ui_[9];
	unique_ptr<Sprite> reload_ui_;
	//弾のUIのステータス
	XMFLOAT2 bullet_pos_[9];
	XMFLOAT2 original_bulletpos_;
	float bullet_rot_[9];
	float time_[9];
	bool drop_bulletflag_[9];
	//Reloadのステータス
	XMFLOAT4 reload_color_{ 1.0f,1.0f,1.0f,0.9f };
	XMFLOAT2 reload_pos_{ WinApp::window_width / 2,210.f };
	XMFLOAT2 reload_size_{ 210.f,140.f };
	XMFLOAT2 reload_oldsize_ = reload_size_;
	float reload_time_{};
	int remaining_{};
	int old_remaining_{};
	bool easing_changeflag_ = false;
};

