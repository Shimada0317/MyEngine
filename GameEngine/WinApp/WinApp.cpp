#include "WinApp.h"
#include<Windows.h>
#include"imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WinApp::WidowProck(HWND hwnd, UINT msg, WPARAM wparm, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparm, lparam)) {
		return 1;
	}
	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破壊された
		PostQuitMessage(0);//OSに対して、アプリの終了を伝える
		return 0;
	}


	return DefWindowProc(hwnd, msg, wparm, lparam);//標準の処理を行う
}

WinApp* WinApp::GetInstance()
{
	static WinApp instance;

	return &instance;
}

void WinApp::Initialize()
{

	//WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WidowProck;//ウィンドウプロシージャを設定
	w.lpszClassName = L"ROBSHOOTING";//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	//ウィンドウサイズ{X座標 Y座標　横幅　縦幅}
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//自動でサイズ補正
//
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,//クラス名
		L"ROBSHOOTING",//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//標準的なウィンドウスタイル
		CW_USEDEFAULT,//表示X座標(OSに任せる)
		CW_USEDEFAULT,//表示Y座標(OSに任せる)
		wrc.right - wrc.left,//ウィンドウ横幅
		wrc.bottom - wrc.top,//ウィンドウ縦幅
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::Finalize()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}

bool WinApp::ProcessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}
