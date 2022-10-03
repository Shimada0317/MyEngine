#pragma once

#include"CollisionPrimitive.h"
#include<Windows.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>

class Collision
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

public:

	///<summary>
	///球と平面の当たり判定
	///</summary>
	///<param name="sphere">球</param>
	///<param name="plane">平面</param>
	///<param name="inter">交点(平面上の最接近点)</param>
	///<returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	///球と三角形の最近接点を求める
	///</summary>
	///<param name="point">点</param>
	///<param name="triangle">三角形</param>
	///<param name="closesr">最近接点(出力用)</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);
	///<summary>
	///球と三角形の最近接点を求める
	///</summary>
	///<param name="point">点</param>
	///<param name="triangle">三角形</param>
	///<param name="inter">交点(三角形上の最近接点)</param>
	/// ///<returns>交差しているか否か</returns>
	static bool CheckSpere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	///レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <return>交差しているか否か</return>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
	///<summary>
	///レイと法線付きの三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <return>交差しているか否か</return>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	void CheckPlayer2Enemy(XMFLOAT3 Playerposition, XMFLOAT3 Playerscale, XMFLOAT3 Enemyposition, XMFLOAT3 Enemyscale,int PlayerHp,int PlayerLevel);

	void CheckPlayer2Boss(XMFLOAT3 Playerposition, XMFLOAT3 Playerscale, XMFLOAT3 Bossposition, XMFLOAT3 Bossscale, int PlayerHp,int PlayerLevel);

	static bool Player2Other(XMVECTOR Playerposition, XMFLOAT3 Playerscale, XMVECTOR Otherposition, XMFLOAT3 Otherscale);

	static bool HeadShot(XMVECTOR Playerposition, XMFLOAT3 Playerscale, XMVECTOR Otherposition, XMFLOAT3 Otherscale);

	static bool BodyHit(XMVECTOR bodyPos, XMFLOAT3 bodyScl, XMVECTOR PlayerPos, XMFLOAT3 playerScl);

	static bool HeadHit(XMVECTOR headPos, XMFLOAT3 headScl, XMVECTOR PlayerPos, XMFLOAT3 playerScl);

	static bool ArmHit(XMVECTOR armPos, XMFLOAT3 armScl, XMVECTOR PlayerPos, XMFLOAT3 PlayerScl);
};

