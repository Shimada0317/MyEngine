#include "Mouse.h"

using namespace DirectX;

Mouse* Mouse::GetInstance()
{
	static Mouse instance;

	return &instance;
}

void Mouse::Initialize(WinApp* winApp)
{
	HRESULT result;

	this->winApp = winApp;

	ComPtr<IDirectInput8> click = nullptr;
	result = DirectInput8Create(
		winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&click, nullptr);

	result = click->CreateDevice(GUID_SysMouse, &Onclick, NULL);
	result = Onclick->SetDataFormat(&c_dfDIMouse);
	result = Onclick->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	ShowCursor(false);
}

void Mouse::Update()
{
	clipre = cli;
	HRESULT result= Onclick->GetDeviceState(sizeof(DIMOUSESTATE), &cli);

	if (FAILED(result)) {
		Onclick->Acquire();
		Onclick->GetDeviceState(sizeof(DIMOUSESTATE), &cli);


	}

	//memcpy(cliPre, cli, sizeof(cli));

//	result = Onclick->Acquire();
}

bool Mouse::PushClick(BYTE click)
{
	if (!(clipre.rgbButtons[click]&(0x80))) {
		if ((cli.rgbButtons[click] & (0x80))) {
			return true;
		}
	}

    return false;
}

void Mouse::MouseMoveSprite(XMFLOAT2& spritePos)
{

	POINT mousePosition;

	GetCursorPos(&mousePosition);

	HWND hwnd = winApp->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);

	spritePos.x = mousePosition.x;
	spritePos.y = mousePosition.y;
}

void Mouse::MouseGetSpritePos(XMFLOAT2& spritePos)
{
	POINT mousePosition;

	mousePosition.x = spritePos.x;
	mousePosition.y = spritePos.y;

	GetCursorPos(&mousePosition);

	HWND hwnd = winApp->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);
}

void Mouse::Mousemove(const XMMATRIX& View, const XMMATRIX& Pro, const XMMATRIX& viewPort, const XMFLOAT2& spritePos, XMVECTOR& positionRet,float Distance=11)
{

	XMVECTOR posNear = { spritePos.x,spritePos.y, 0 };
	XMVECTOR posFar = { spritePos.x, spritePos.y,1 };

	//ビュー、プロジェクション、ビューポート3つの行列の乗算
	//XMMATRIX matVPV = View * Pro * viewPort;
	XMMATRIX InverseviewPort= XMMatrixInverse(nullptr, viewPort);

	XMMATRIX InversePro= XMMatrixInverse(nullptr, Pro);

	XMMATRIX InverseView= XMMatrixInverse(nullptr, View);

	XMMATRIX mataa = View* Pro * viewPort;

	XMMATRIX Inversemataa = XMMatrixInverse(nullptr, mataa);

	posNear = XMVector3TransformCoord(posNear, Inversemataa);
	posFar = XMVector3TransformCoord(posFar,Inversemataa);

	XMVECTOR mouseDirection = posNear - posFar;
	mouseDirection = XMVector3Normalize(mouseDirection);

	const float kDistanceTestObject = Distance;

	positionRet = posNear - mouseDirection * kDistanceTestObject ;

	//player->SetPosition(positionRet);

}
	