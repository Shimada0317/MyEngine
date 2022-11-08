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
	if (!(clipre.rgbButtons[click]&(0x80))&&(cli.rgbButtons[click]&(0x80))) {
		return true;
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

void Mouse::Mousemove(const XMMATRIX& ViewPro, const XMMATRIX& viewPort, const XMFLOAT2& spritePos, XMVECTOR& positionRet)
{
	/*POINT mousePosition;

	GetCursorPos(&mousePosition);

	HWND hwnd = winApp->GetHwnd();

	ScreenToClient(hwnd, &mousePosition);


	retpos.x = mousePosition.x;
	retpos.y = mousePosition.y;

	spriteRet->SetPosition(retpos);*/

	//ビューポート行列
	//XMMATRIX matViewport;

	//ChangeViewPort(matViewport);

	//XMMATRIX ViewPro = camera->GetViewProjectionMatrix();

	//ビュー、プロジェクション、ビューポート3つの行列の乗算
	XMMATRIX matVPV = ViewPro * viewPort;

	XMMATRIX matIverserVPV = XMMatrixInverse(nullptr, matVPV);


	XMVECTOR posNear = { spritePos.x,spritePos.y, 0,1 };
	XMVECTOR posFar = { spritePos.x, spritePos.y,1,1 };

	posNear = XMVector3TransformCoord(posNear, matIverserVPV);
	posFar = XMVector3TransformCoord(posFar, matIverserVPV);

	XMVECTOR mouseDirection = posNear - posFar;
	mouseDirection = XMVector3Normalize(mouseDirection);

	const float kDistanceTestObject = 10;

	

	positionRet = posNear - mouseDirection * kDistanceTestObject;

	//player->SetPosition(positionRet);

}
	