#pragma once
#include"HelperMath.h"
#include"Sprite.h"
#include"WinApp.h"

class BulletUI
{
public:
	~BulletUI();
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="remaining">残弾数</param>
	/// <param name="bulletuipos">弾のUIも座標</param>
	/// <param name="reloadpos">リロードの文字の座標</param>
	void Create(int remaining,const XMFLOAT2& bulletuipos,const XMFLOAT2& reloadpos);
	/// <summary>
	/// ステータスのセット
	/// </summary>
	void Set();
	/// <summary>
	/// 弾を発砲したときの処理
	/// </summary>
	/// <param name="remaining">残弾数</param>
	void Shot(int remaining);
	/// <summary>
	/// 弾のセット
	/// </summary>
	/// <param name="remaining">残弾数</param>
	void SetRemainig(int remaining) { this->remaining_ = remaining; };
	/// <summary>
	/// リロードの処理
	/// </summary>
	/// <param name="remaining">残弾数</param>
	void Reload(int  remaining);
	/// <summary>
	/// 落ちていく処理
	/// </summary>
	void FallingUI();
	/// <summary>
	/// リロードの文字処理
	/// </summary>
	void ReloadMotion();
	/// <summary>
	/// 描画
	/// </summary>
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
	//イージング用のフラグとタイマー
	float reload_time_{};
	bool easing_changeflag_ = false;
	//残弾数の過去の残弾数
	int remaining_{};
	int old_remaining_{};
};

