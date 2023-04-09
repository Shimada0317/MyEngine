#pragma once
#include<wrl.h>
#include<DirectXMath.h>

using namespace DirectX;

class HelperMath
{
public:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns></returns>
	static HelperMath* GetInstance();

	/// <summary>
	/// XMVECTOR���m�̉��Z
	/// </summary>
	XMVECTOR XMvectorAddXMvector(const XMVECTOR& FirstValue, const XMVECTOR& AddValue);
	/// <summary>
	/// XMVECTOR���m�̌��Z
	/// </summary>
	XMVECTOR XMvectorSubXMVector(const XMVECTOR& FirstValue, const XMVECTOR& SubValue);
	/// <summary>
	/// XMFLOAT3���m�̉��Z
	/// </summary>
	XMFLOAT3 XMFLOAT3AddXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& AddValue);
	/// <summary>
	/// XMFLOAT3���m�̌��Z
	/// </summary>
	XMFLOAT3 XMFLOAT3SubXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& SubValue);
	/// <summary>
	/// XMFLOAT3�̐����̔��]
	/// </summary>
	XMFLOAT3 XMFLOAT3ChangeValue(const XMFLOAT3& ChangeValue);
	/// <summary>
	/// �S�ē����l�ɒu��������
	/// </summary>
	/// <param name="SourceValue">����������l</param>
	/// <returns></returns>
	XMFLOAT3 ConvertToXMFLOAT3(float SourceValue);

	void ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset);
};
