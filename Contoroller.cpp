#include "Contoroller.h"
#include<wrl.h>
#include"WinApp.h"
//#define DIRECTINPUT_VERSION 0x0800
//#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

LPDIRECTINPUT8 g_InputInterface;							//!< DIRECTINPUT8のポインタ
LPDIRECTINPUTDEVICE8 g_GamePadDevice;					//!< DIRECTINPUTDEVICE8のポインタ
DIDEVCAPS g_diDevCaps;


HWND hWnd;

using namespace Microsoft::WRL;

BOOL CleanUpDirectInput()
{
	if (g_InputInterface != NULL) {
		g_InputInterface->Release();
	}

	return TRUE;
}

BOOL CALLBACK EnumaAxesCallBack(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
	HRESULT result;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;//十字キー最小値(↑、←)
	diprg.lMax = 0 + 1000;//十字キー最大値(↓,→)
	result = g_GamePadDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(result)) {
		//失敗
		return DIENUM_STOP;
	}
	//成功
	return DIENUM_CONTINUE;
}


BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) {
	HRESULT result;
	result = g_InputInterface->CreateDevice(
		pdidInstance->guidInstance,
		&g_GamePadDevice,
		NULL
	);

	if (FAILED(result)) {
		return DIENUM_CONTINUE;
	}
	g_GamePadDevice->SetDataFormat(&c_dfDIJoystick2);
	g_GamePadDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	g_GamePadDevice->GetCapabilities(&g_diDevCaps);
	g_GamePadDevice->EnumObjects(EnumaAxesCallBack, (VOID*)hWnd, DIDFT_AXIS);
	g_GamePadDevice->Acquire();

	return DIENUM_STOP;
}



void Contoroller::Initialize(WinApp* winApp)
{
	HRESULT result;

	this->winApp = winApp;

	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput = nullptr;
	result = DirectInput8Create(
		winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	result = DirectInput8Create(
		winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_InputInterface, NULL);

	result = g_InputInterface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ALLDEVICES
	);

	//全キーの入力情報を取得する
	g_GamePadDevice->Poll();
	result = g_GamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), &joys);


	//ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	CleanUpDirectInput();
}

void Contoroller::Update()
{
	HRESULT result;

	memcpy(keyPre, key, sizeof(key));

	//情報取得
	result = g_GamePadDevice->Acquire();

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();

	//BYTE key[256] = {};
	result = devkeyboard->GetDeviceState(sizeof(key), key);

}



bool Contoroller::Pushjoys(DIJOYSTATE joy)
{
	for (int i = 0; i < 32; i++) {
		if (joy.rgbButtons[i] & 0x80) {
			return true;
		}
	}
	return false;
}