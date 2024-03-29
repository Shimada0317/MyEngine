#include<Windows.h>

#pragma once
class WinApp
{
public:
	static WinApp* GetInstance();

	//初期化
	void Initialize();
	//更新
	void Update();

	void Finalize();

	bool ProcessMessage();

	static LRESULT WidowProck(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHinstance() { return w.hInstance; }

	static const int window_width = 1280;
	static const int window_height = 720;
private:
	HWND hwnd = nullptr;

	WNDCLASSEX w{};

	
};

