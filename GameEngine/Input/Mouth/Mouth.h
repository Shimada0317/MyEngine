#pragma once
#include<Windows.h>
#include<wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"WinApp.h"
#include<vector>
#include<DirectXMath.h>

class Mouth
{
public:
	struct MouthPos {
		LONG lX;
		LONG lY;
	};

private:
	WinApp* winApp = nullptr;
public:
	//namespaceè»ó™
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public://ÉÅÉìÉoä÷êî
	static Mouth* GetInstance();

	//èâä˙âª
	void Initialize(WinApp* winApp);
	//çXêV
	void Update();

	bool PushClick(BYTE click);

	void MouthMoveSprite(XMFLOAT2& spritePos);

	void RecoilMouth(XMFLOAT2& spritepos);

	void MouthGetSpritePos(XMFLOAT2& spritePos);

	void Mouthmove(const XMMATRIX& view,const XMMATRIX& Pro,const XMMATRIX& viewPort,const XMFLOAT2& spritePos,XMVECTOR& positionRet,float Distance);

	MouthPos& GetPosition() { this->pos; }

	MouthPos GetMouthMove();
private:

	ComPtr<IDirectInputDevice8> devkeyboard;
	ComPtr<IDirectInputDevice8> Onclick;

	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	DIMOUSESTATE2 cli = {};
	DIMOUSESTATE2 clipre = {};

	POINT pos;


};

