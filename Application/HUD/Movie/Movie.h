#pragma once
#include"HelperMath.h"
#include"Sprite.h"

class Movie
{
public:

	~Movie();
	/// <summary>
	/// 生成
	/// </summary>
	void Create();
	/// <summary>
	/// ステータスのセット
	/// </summary>
	void StatusSet();
	/// <summary>
	/// 表示
	/// </summary>
	void Disply();
	/// <summary>
	/// 非表示
	/// </summary>
	void Invisible();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	unique_ptr<Sprite> curtainup_;
	unique_ptr<Sprite> curtaindown_;
	unique_ptr<Sprite> skip_;

	//始まりと終わりの演出使う黒いカーテン
	XMFLOAT2 curtainuppos_ = { 0.0f,0.0f };
	XMFLOAT2 curtainsize_ = { 1280.0f,100.0f };
	XMFLOAT2 curtaindownpos_ = { 0.0f,620.0f };
	//Skip文字の表示
	XMFLOAT2 skippos_ = { 0.0f,620.0f };
};

