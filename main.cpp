#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<vector>
#include<string>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include<dinput.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<d3dx12.h>
#include<time.h>
#include"Input.h"
#include"WinApp.h"
#include"Sprite.h"
#include"DirectXCommon.h"
#include"GameScene.h"
#include"Object3d.h"
#include"Model.h"
#include"Contoroller.h"
#include"Audio.h"
#include"Collision.h"
#include"MyGame.h"


#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

const int spriteSRVCount = 512;


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame game;
	game.Initialize();
	while (true)//ゲームループ
	{
		game.Update();
		if (game.GetFinish()) {
			break;
		}
		game.Draw();
	}
	game.Finalize();
	return 0;
}
