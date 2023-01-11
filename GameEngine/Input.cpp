#include"Input.h"
#include<wrl.h>
#include"WinApp.h"
//#define DIRECTINPUT_VERSION 0x0800
//#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace Microsoft::WRL;

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	HRESULT result;

	this->winApp = winApp;

	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput = nullptr;
	result = DirectInput8Create(
		winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);


	//�S�L�[�̓��͏����擾����

	//ComPtr<IDirectInputDevice8> devkeyboard = nullptr;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);


	//
	ComPtr<IDirectInput8> click = nullptr;
	result = DirectInput8Create(
		winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&click, nullptr);

	result = click->CreateDevice(GUID_SysMouse, &Onclick, NULL);
	result = Onclick->SetDataFormat(&c_dfDIMouse);
	result = Onclick->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	

}

void Input::Updata() 
{
	HRESULT result;

	memcpy(keyPre, key, sizeof(key));

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();

	result = Onclick->Acquire();

	//BYTE key[256] = {};
	result = devkeyboard->Acquire();

	result = devkeyboard->GetDeviceState(sizeof(key), key);

	result = Onclick->GetDeviceState(sizeof(DIMOUSESTATE), &cli);

	
}

bool Input::PushKey(BYTE keyNumber)
{
	if (key[keyNumber]) {
		return true;
	}
	
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (key[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	return false;
}

Input::MousePos Input::GetMouseMove()
{
	MousePos tmp;
	tmp.lX = cli.lX;
	tmp.lY = cli.lY;

	return tmp;
}


