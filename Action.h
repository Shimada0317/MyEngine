#pragma once
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include"Input.h"

class Action
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
private:
	Action();
	
	~Action();

public:
	Action(const Action&) = delete;

	Action& operator=(const Action&) = delete;

	static Action* GetInstance();

	void PlayerMove3d(XMFLOAT3 &position,float Speed);

	void PlayerJump(XMFLOAT3& position,bool& JumpFlag);

	void PlayerMove2d(XMFLOAT3 &position, float Speed);


	void Gunshot(int bullet,bool& shot);

	bool Shot(int bullet, bool& shot);
private:
	float JumpPosition = 0;
};

