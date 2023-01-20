//#pragma once
//
//#include<DirectXMath.h>
//
//class PointLight
//{
//private://エイリアス
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMVECTOR = DirectX::XMVECTOR;
//	using XMMATRIX = DirectX::XMMATRIX;
//public://サブクラス
//	struct ConstBufferData
//	{
//		XMFLOAT3 lightpos;
//		float pad1;
//		XMFLOAT3 lightcolor;
//		float pad2;
//		XMFLOAT3 lightatten;
//		unsigned int active;
//	};
//
//public://メンバ関数
//	inline void SetLightPos(const XMFLOAT3& lightpos) { this->lightpos = lightpos; }
//};
//
