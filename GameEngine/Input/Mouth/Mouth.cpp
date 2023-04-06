#include "Mouth.h"

using namespace DirectX;

Mouth* Mouth::GetInstance()
{
	static Mouth instance;

	return &instance;
}

void Mouth::Initialize(WinApp* winApp)
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
	
	//ShowCursor(false);
}

void Mouth::Update()
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

bool Mouth::PushClick(BYTE click)
{
	if (!(clipre.rgbButtons[click]&(0x80))) {
		if ((cli.rgbButtons[click] & (0x80))) {
			return true;
		}
	}

    return false;
}

void Mouth::MouthMoveSprite(XMFLOAT2& spritePos)
{
	POINT mousePosition;

	GetCursorPos(&mousePosition);

	HWND hwnd = winApp->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);

	spritePos.x = mousePosition.x;
	spritePos.y = mousePosition.y;
}

void Mouth::RecoilMouth(XMFLOAT2& spritepos)
{
	HWND hwnd = winApp->GetHwnd();

	float posX = 0;
	float posY = 0;

	WINDOWINFO windoInfo;
	//ウィンドウの位置を取得
	windoInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hwnd, &windoInfo);
	//マウスの移動先の絶対座標
	posX = spritepos.x + windoInfo.rcWindow.left + 10;
	posY = spritepos.y + windoInfo.rcWindow.top + 5;


	SetCursorPos(posX, posY);
}

void Mouth::MouthGetSpritePos(XMFLOAT2& spritePos)
{
	POINT mousePosition;

	mousePosition.x = spritePos.x;
	mousePosition.y = spritePos.y;

	GetCursorPos(&mousePosition);

	HWND hwnd = winApp->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);
}

void Mouth::Mouthmove(const XMMATRIX& view, const XMMATRIX& Pro, const XMMATRIX& viewPort, const XMFLOAT2& spritePos, XMVECTOR& positionRet, float Distance)
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

	