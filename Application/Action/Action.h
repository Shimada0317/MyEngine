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
	using XMVECTOR = DirectX::XMVECTOR;
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Action();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Action();
public:
	Action(const Action&) = delete;

	Action& operator=(const Action&) = delete;
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static Action* GetInstance();
	/// <summary>
	/// 3Dアクションゲーム用、移動用関数(3Dだけど2Dアクションゲーム想定)
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	void PlayerMove3d(XMVECTOR& position);
	/// <summary>
	/// 3Dアクションゲーム用、ジャンプ用関数
	/// </summary>
	/// <param name="position">プレイヤーの座標</param>
	/// <param name="JumpFlag">ジャンプをしているか否か</param>
	void PlayerJump(XMFLOAT3& position, bool& JumpFlag);
	/// <summary>
	/// 2Dアクションゲーム用、移動用関数
	/// </summary>
	/// <param name="position"></param>
	/// <param name="Speed"></param>
	void PlayerMove2d(XMFLOAT2& position, float Speed);
	/// <summary>
	/// ロボットの更新処理
	/// </summary>
	/// <param name="minValue">最小の範囲値</param>
	/// <param name="maxValue">最大の範囲値</param>
	float GetRangRand(float minValue, float maxValue);
};

