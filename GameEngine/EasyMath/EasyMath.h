#pragma once
#include<wrl.h>
#include<DirectXMath.h>

using namespace DirectX;

class EasyMath
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static EasyMath* GetInstance();

	/// <summary>
	/// XMVECTOR同士の加算
	/// </summary>
	XMVECTOR XMvectorAddXMvector(const XMVECTOR& FirstValue, const XMVECTOR& AddValue);
	/// <summary>
	/// XMVECTOR同士の減算
	/// </summary>
	XMVECTOR XMvectorSubXMVector(const XMVECTOR& FirstValue, const XMVECTOR& SubValue);
	/// <summary>
	/// XMFLOAT3同士の加算
	/// </summary>
	XMFLOAT3 XMFLOAT3AddXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& AddValue);
	/// <summary>
	/// XMFLOAT3同士の減算
	/// </summary>
	XMFLOAT3 XMFLOAT3SubXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& SubValue);
	/// <summary>
	/// XMFLOAT3の正負の反転
	/// </summary>
	XMFLOAT3 XMFLOAT3ChangeValue(const XMFLOAT3& ChangeValue);
	/// <summary>
	/// 全て同じ値に置き換える
	/// </summary>
	/// <param name="SourceValue">書き換える値</param>
	/// <returns></returns>
	XMFLOAT3 ConvertToXMFLOAT3(float SourceValue);
};

