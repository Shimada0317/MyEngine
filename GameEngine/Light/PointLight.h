#pragma once

#include<DirectXMath.h>

class PointLight
{
private://エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public://サブクラス
	struct ConstBufferData
	{
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		unsigned int active;
	};

public://メンバ関数
	inline void SetLightPos(const XMFLOAT3& lightpos) { this->LightPos = lightpos; }

	inline const XMFLOAT3& GetLightPos() { return LightPos; }
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->LightColor = lightcolor; }
	inline const XMFLOAT3& GetLightColor() { return LightColor; }
	inline void SetLightAtten(const XMFLOAT3& lightatten) { this->LightAtten = lightatten; }
	inline const XMFLOAT3& GetLightAtten() { return LightAtten; }
	inline void SetActive(bool active) { this->Active = active; }
	inline bool IsActive() { return Active; }
private:
	//ライトの座標
	XMFLOAT3 LightPos = { 0.0f,0.0f,0.0f };
	//ライト色
	XMFLOAT3 LightColor = { 1.0f,1.0f,1.0f };
	//ライト距離減衰係数
	XMFLOAT3 LightAtten = { 1.0f,1.0f,1.0f };
	//有効フラグ
	bool Active = false;

};

