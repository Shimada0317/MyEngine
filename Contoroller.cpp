#include "Contoroller.h"
#include<wrl.h>
#include"WinApp.h"
//#define DIRECTINPUT_VERSION 0x0800
//#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

LPDIRECTINPUT8 g_InputInterface;							//!< DIRECTINPUT8�̃|�C���^
LPDIRECTINPUTDEVICE8 g_GamePadDevice;					//!< DIRECTINPUTDEVICE8�̃|�C���^
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
	diprg.lMin = 0 - 1000;//�\���L�[�ŏ��l(���A��)
	diprg.lMax = 0 + 1000;//�\���L�[�ő�l(��,��)
	result = g_GamePadDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(result)) {
		//���s
		return DIENUM_STOP;
	}
	//����
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

	//DirectInput�̃C���X�^���X����
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

	//�S�L�[�̓��͏����擾����
	g_GamePadDevice->Poll();
	result = g_GamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), &joys);


	//ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	CleanUpDirectInput();
}

void Contoroller::Update()
{
	HRESULT result;

	memcpy(keyPre, key, sizeof(key));

	//���擾
	result = g_GamePadDevice->Acquire();

	//�L�[�{�[�h���̎擾�J�n
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