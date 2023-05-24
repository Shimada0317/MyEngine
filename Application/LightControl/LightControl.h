#pragma once
#include"LightGroup.h"
#include"Object3d.h"

class LightControl
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	void Initialize();

	void Set();

	void Update();

	void WrappingLight();

	void FieldLightColorChange();

	void SetFieldLightColor(const XMFLOAT3& color){this->fieldspotlightcolor_=color;}

	void SetNotSpotLightActive(int number) { lightgroupe_->SetSpotLightActive(number, false); }

	void SetFieldLightPos(const XMFLOAT3& fieldspotlightpos) { this->fieldspotlightpos_ = fieldspotlightpos; }
private:
	unique_ptr<LightGroup>lightgroupe_;

	//フィールドライト用の変数
	XMFLOAT3 fieldspotlightdir_ = { 0,-10,0 };
	XMFLOAT3 fieldspotlightpos_ = { 0,505,50 };
	XMFLOAT3 fieldspotlightcolor_ = { 0.9f,0.5f,0.f };
	XMFLOAT3 fieldspotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 fieldspotlightfactorangle_ = { 20.0f,30.0f };
	//プレイヤー用のライトの変数
	XMFLOAT3 playerspotlightdir_ = { 0,-20,0 };
	XMFLOAT3 playerspotlightpos_ = { 0,35,0 };
	XMFLOAT3 playerspotlightcolor_ = { 1.f,1.f,1.f };
	XMFLOAT3 playerspotlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 playerspotlightfactorangle_ = { 20.0f,30.0f };
	//ステージ内を動いているライトの変数
	XMFLOAT3 searchlightdir_[3];
	XMFLOAT3 searchlightpos_[3];
	XMFLOAT3 searchlightcolor_[3];
	XMFLOAT3 searchlightatten_ = { 0.f,0.f,0.f };
	XMFLOAT2 searchlightfactorangle_ = { 20.0f,30.0f };

	bool easing_ = true;
	float easingwaittimer_ = 0.f;
	bool easingchange_ = false;
	float time_ = -1.0f;
	float lightdireasingtime_ = 0.f;
	float colortime_ = 1.0f;
	float colortimered_ = 0.0f;
};

