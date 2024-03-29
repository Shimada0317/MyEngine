#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"WinApp.h"
#include<vector>
#include<DirectXMath.h>


class Input
{
public:
	struct MousePos {
		LONG lX;
		LONG lY;
	};

private:
	WinApp* winApp = nullptr;
public:
	//namespace省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数

	static Input* GetInstance();

	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();

	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);

	MousePos& GetPosition() {this->pos;}

	MousePos GetMouseMove();
private:

	ComPtr<IDirectInputDevice8> devkeyboard;
	ComPtr<IDirectInputDevice8> Onclick;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	DIMOUSESTATE2 cli = {};
	DIMOUSESTATE2 clepre = {};

	POINT pos;
	
};