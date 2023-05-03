#pragma once
#include"Sprite.h"
#include"HelperMath.h"
#include"WinApp.h"

class Continue
{
public:
	~Continue();
	/// <summary>
	/// ����
	/// </summary>
	void Create(const XMFLOAT2& yesposition_,const XMFLOAT2& noposition_);
	/// <summary>
	/// �F�̕ύX
	/// </summary>
	/// <param name="yescolor_">Yes�̐F</param>
	/// <param name="nocolor_">No�̐F</param>
	void ChangeColor(const XMFLOAT4& yescolor_, const XMFLOAT4& nocolor_);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	unique_ptr<Sprite> continue_;
	unique_ptr<Sprite> yes_;
	unique_ptr<Sprite> no_;

	XMFLOAT2 yesposition_{ WinApp::window_width / 2 - 200,WinApp::window_height / 2 + 100 };
	XMFLOAT2 noposition_{ WinApp::window_width / 2 + 200,WinApp::window_height / 2 + 100 };
	XMFLOAT4 yescolor_{ 1.f,1.f,1.f,1.f };
	XMFLOAT4 nocolor_{ 1.f,1.f,1.f,1.f };
};

