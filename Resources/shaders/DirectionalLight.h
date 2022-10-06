#pragma once
#include<DirectXMath.h>

class DirectionalLight
{
private://エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct ConstBufferData
	{
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
		unsigned int active;
	};
public:
	inline void SetLightDir(const XMVECTOR& lightdir) { this->lightdir = DirectX::XMVector3Normalize(lightdir); }

	inline const XMVECTOR& GetLightDir() { return lightdir; }
	
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }

	inline const XMFLOAT3& GetLightColor() { return lightcolor; }

	inline void SetActive(bool active) { this->active = active; }

	inline bool IsActive() { return active; }

private:
	XMVECTOR lightdir = { 1,0,0,0 };

	XMFLOAT3 lightcolor = { 1,1,1 };

	bool active = false;
};

