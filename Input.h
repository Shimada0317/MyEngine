#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"WinApp.h"


class Input
{
private:
	WinApp* winApp = nullptr;
public:
	//namespaceÈ—ª
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://ƒƒ“ƒoŠÖ”
	//‰Šú‰»
	void Initialize(WinApp* winApp);
	//XV
	void Update();

	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);

private:

	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};


};