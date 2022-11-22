#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"WinApp.h"
#include<vector>
#include<DirectXMath.h>

class Mouse
{
public:
	struct MousePos {
		LONG lX;
		LONG lY;
	};

private:
	WinApp* winApp = nullptr;
public:
	//namespace�ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public://�����o�֐�
	static Mouse* GetInstance();

	//������
	void Initialize(WinApp* winApp);
	//�X�V
	void Updata();

	bool PushClick(BYTE click);

	void MouseMoveSprite(XMFLOAT2& spritePos);

	void Mousemove(const XMMATRIX& viewPro,const XMMATRIX& viewPort,const XMFLOAT2& spritePos,XMVECTOR& positionRet);

	MousePos& GetPosition() { this->pos; }

	MousePos GetMouseMove();
private:

	ComPtr<IDirectInputDevice8> devkeyboard;
	ComPtr<IDirectInputDevice8> Onclick;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	DIMOUSESTATE2 cli = {};
	DIMOUSESTATE2 clipre = {};

	POINT pos;


};
