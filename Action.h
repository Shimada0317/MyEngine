#pragma once
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include"Input.h"

class Action
{
private: // ?G?C???A?X
// Microsoft::WRL::???ȗ?
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::???ȗ?
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static Action* GetInstance();

	void PlayerMove3d(XMFLOAT3 &position,float Speed);

	void PlayerJump(XMFLOAT3& position,bool& JumpFlag);

	void PlayerMove2d(XMFLOAT2 position, float Speed);


	void Gunshot(bool alive, bool havegun, int bullet,bool shot);
private:
	float JumpPosition = 0;
};

