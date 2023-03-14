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
	/// <summary>
	/// イーズアウト
	/// </summary>
	/// /// <param name="MoveValue">移動する値</param>
	/// <param name="PurposeValue">目的地の値</param>
	void EaseOut(float& MoveValue, float PurposeValue);

	/// <summary>
	/// 透明にする
	/// </summary>
	/// <param name="color">減色</param>
	/// <param name="transparent">α値</param>
	void Flash(float& color,float transparent);

	/// <summary>
	/// 投げ上げ
	/// </summary>
	/// <param name="gravity">重力</param>
	/// <param name="time">時間</param>
	/// <param name="upper">投げ上げる値</param>
	/// <param name="position">現在地</param>
	void ThrowUp(float gravity, float time, float upper, float& position);

	/// <summary>
	/// XMFLOAT3の全要素に同じ値を加算する
	/// </summary>
	/// <param name="value">加算される値</param>
	/// <param name="addvalue">加算する値</param>
	/// <returns></returns>
	DirectX::XMFLOAT3 ConvertToXMFLOAT3(float SourceValue);

	/// <summary>
	/// ベクター同士の比較
	/// </summary>
	/// <param name="FirstValue">1つ目の値</param>
	/// <param name="SecondValue">2つ目の値</param>
	/// <returns></returns>
	bool CompletelyTogetherXMVECTOR(const XMVECTOR& FirstValue, const XMVECTOR& SecondValue);

	void DebugMove(XMFLOAT3& Value);

	void XMvectorAddXMvector(XMVECTOR& FirstValue, const XMVECTOR& AddValue);

	void XMvectorSubXMVector(XMVECTOR& FirstValue, const XMVECTOR& AddValue);
private:
};

