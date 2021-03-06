#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"WinApp.h"

class Contoroller
{
private:
	WinApp* winApp = nullptr;
public:
	//namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();

	bool Pushjoys(DIJOYSTATE joys);

	bool PushUp();
private:

	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	DIJOYSTATE joys;
};

