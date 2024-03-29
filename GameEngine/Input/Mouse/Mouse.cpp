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

	spritePos.x = (float)mousePosition.x;
	spritePos.y = (float)mousePosition.y;
}

void Mouse::RecoilMouse(XMFLOAT2& spritepos)
{
	HWND hwnd = winApp->GetHwnd();

	float posX = 0;
	float posY = 0;

	WINDOWINFO windoInfo;
	//ウィンドウの位置を取得
	windoInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windoInfo);
	//マウスの移動先の絶対座標
	posX = spritepos.x + windoInfo.rcWindow.left + 10.f;
	posY = spritepos.y + windoInfo.rcWindow.top + 5.f;


	SetCursorPos((int)posX, (int)posY);
}

//void Mouse::MouseGetSpritePos(XMFLOAT2& spritePos)
//{
//	POINT mousePosition{};
//
//	mousePosition.x = spritePos.x;
//	mousePosition.y = spritePos.y;
//
//	GetCursorPos(&mousePosition);
//
//	HWND hwnd = winApp->GetHwnd();
//
//	ScreenToClient(hwnd, &mousePosition);
//}

void Mouse::Mousemove(const XMMATRIX& view, const XMMATRIX& Pro, const XMMATRIX& viewPort, const XMFLOAT2& spritePos, XMVECTOR& positionRet, float Distance)
{
	XMVECTOR posNear = { spritePos.x,spritePos.y, 0 };
	XMVECTOR posFar = { spritePos.x, spritePos.y,1 };

	//ビュー、プロジェクション、ビューポート3つの行列の乗算
	//XMMATRIX matVPV = View * Pro * viewPort;
	XMMATRIX InverseviewPort = XMMatrixInverse(nullptr, viewPort);

	XMMATRIX InversePro = XMMatrixInverse(nullptr, Pro);

	XMMATRIX InverseView = XMMatrixInverse(nullptr, view);

	XMMATRIX mataa = view * Pro * viewPort;

	XMMATRIX Inversemataa = XMMatrixInverse(nullptr, mataa);

	posNear = XMVector3TransformCoord(posNear, Inversemataa);
	posFar = XMVector3TransformCoord(posFar, Inversemataa);

	XMVECTOR mouseDirection = posNear - posFar;
	mouseDirection = XMVector3Normalize(mouseDirection);

	const float kDistanceTestObject = Distance;

	positionRet = posNear - mouseDirection * kDistanceTestObject;

}

	