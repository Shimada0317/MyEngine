#pragma once
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include"Input.h"
#include"HelperMath.h"

using namespace DirectX;

class Camera;

class Action
{
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
	///  乱数生成
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

	void EasingAddTimer(float& timer_,float addtime_);

	float EasingOut(float time, float addvalue);

	XMFLOAT3 EasingOutXMFLOAT3(float time, const XMFLOAT3& addvalue);

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
	/// 画面の揺れ
	/// </summary>
	/// <param name="shakevalue"></param>
	/// <param name="shakingtime"></param>
	/// <param name="eyerot_"></param>
	/// <param name="shakingstartflag_"></param>
	void ScreenShake(float& shakevalue, float shakingtime,XMFLOAT3& eyerot_,bool& shakingstartflag_);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="eyerot_"></param>
	void MoveShakingHead(XMFLOAT3& eyerot_);

	/// <summary>
	/// ベクター同士の比較
	/// </summary>
	/// <param name="FirstValue">1つ目の値</param>
	/// <param name="SecondValue">2つ目の値</param>
	/// <returns></returns>
	bool CompletelyTogetherXMVECTOR(const XMVECTOR& FirstValue, const XMVECTOR& SecondValue);

	void DebugMove(XMFLOAT3& Value);

	void LoopTimer(float& timer,float addvalue,float duration_);

	void ColorDown(XMFLOAT4& color,float downval);

	void ColorUp(XMFLOAT4& color,float upval);

	XMFLOAT3 RandMax(const XMFLOAT3& value,float randvel);

	XMFLOAT3 RandMinAndMax(const XMFLOAT3& minvalue,const XMFLOAT3& maxvalue);

	XMFLOAT2 WorldToScreen(const XMMATRIX& centermat, const XMVECTOR& position,Camera* camera);
private:
	float shakelimittime_ = 0.f;
	float shakingscreenvalue_ = 0.f;
	bool shakingscreenflag_ = false;

	bool shake_ = true;

	bool easing_invert_ = false;
};

