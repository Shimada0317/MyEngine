#include "LightControl.h"
#include"Action.h"

void LightControl::Initialize()
{
	lightgroupe_ = LightGroup::Create();

	for (int i = 0; i < 3; i++) {
		searchlightdir_[i] = { 0,-10,0 };
		searchlightcolor_[i] = { 1.f,1.f,1.f };
	}
	searchlightpos_[0] = { 0, 20, 20 };
	searchlightpos_[1] = { 20, 10, 45 };
	searchlightpos_[2] = { 54,10,43 };

	//使うライトをアクティブ
	lightgroupe_->SetSpotLightActive(0, true);
	lightgroupe_->SetSpotLightActive(1, true);
	lightgroupe_->SetSpotLightActive(2, true);
	lightgroupe_->SetSpotLightActive(3, true);
	lightgroupe_->SetSpotLightActive(4, true);

	//ライトセット
	Object3d::SetLightGroup(lightgroupe_.get());
}

void LightControl::Set()
{
	//フィールド全体を照らすステータス
	lightgroupe_->SetSpotLightDir(0, XMVECTOR({ fieldspotlightdir_.x, fieldspotlightdir_.y, fieldspotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(0, fieldspotlightpos_);
	lightgroupe_->SetSpotLightColor(0, fieldspotlightcolor_);
	lightgroupe_->SetSpotLightAtten(0, fieldspotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(0, fieldspotlightfactorangle_);
	//スタート地点のステータス
	lightgroupe_->SetSpotLightDir(1, XMVECTOR({ playerspotlightdir_.x, playerspotlightdir_.y, playerspotlightdir_.z }));
	lightgroupe_->SetSpotLightPos(1, playerspotlightpos_);
	lightgroupe_->SetSpotLightColor(1, playerspotlightcolor_);
	lightgroupe_->SetSpotLightAtten(1, playerspotlightatten_);
	lightgroupe_->SetSpotLightFactorAngle(1, playerspotlightfactorangle_);
	//フィールド内を動くライトのステータス
	for (int i = 2; i < 5; i++) {
		lightgroupe_->SetSpotLightDir(i, XMVECTOR({ searchlightdir_[i - 2].x, searchlightdir_[i - 2].y, searchlightdir_[i - 2].z }));
		lightgroupe_->SetSpotLightPos(i, searchlightpos_[i - 2]);
		lightgroupe_->SetSpotLightColor(i, searchlightcolor_[i - 2]);
		lightgroupe_->SetSpotLightAtten(i, searchlightatten_);
		lightgroupe_->SetSpotLightFactorAngle(i, searchlightfactorangle_);
	}
}

void LightControl::Update()
{
	WrappingLight();

	Set();

	lightgroupe_->Update();
}

void LightControl::WrappingLight()
{
	const float kDuration = 1;
	const float kStartPointZ = 50.f;
	const float kStartPointX = -30;
	const float kStartPointZ2 = 50.f;
	const float kEndPointZ = 0.f;
	const float kEndPointX = 30;
	const float kEndPointZ2 = 90.0f;


	if (easing_ == false) {
		easingwaittimer_ += 0.1f;
		if (easingwaittimer_ >= 1) {
			easing_ = true;
			easingwaittimer_ = 0.f;
		}
	}
	else if (easing_ == true) {
		Action::GetInstance()->LoopTimer(time_, 0.01f, kDuration);
		if (time_ < -kDuration || time_ > kDuration) {
			easing_ = false;
		}
	}
	//移動方向を反転する
	lightdireasingtime_ += 0.05f;
	//ライトの動き
	searchlightdir_[0].x = Action::GetInstance()->EasingOut(lightdireasingtime_, 5 - 0);
	searchlightdir_[0].z = Action::GetInstance()->EasingOut(time_, kEndPointZ - kStartPointZ);
	searchlightdir_[1].z = Action::GetInstance()->EasingOut(lightdireasingtime_, 5 - 0);
	searchlightdir_[1].x = Action::GetInstance()->EasingOut(time_, kEndPointX - kStartPointX);
	searchlightdir_[2].x = Action::GetInstance()->EasingOut(lightdireasingtime_, 5 - 0);
	searchlightdir_[2].z = Action::GetInstance()->EasingOut(time_, kEndPointZ2 - kStartPointZ2);
}

void LightControl::FieldLightColorChange()
{
	const float kStartColor = -0.5f;
	const float kStartColorRed = 0.0f;
	const float kEndColor = 0.0f;
	const float kEndColorRed = 0.8f;
	if (colortime_ >= 0) {
		colortime_ -= 0.01f;
	}
	if (colortimered_ <= 1) {
		colortimered_ += 0.01f;
	}
	fieldspotlightcolor_.x = Action::GetInstance()->EasingOut(colortimered_, kEndColorRed - kStartColorRed) + 0.9f;
	fieldspotlightcolor_.y = Action::GetInstance()->EasingOut(colortime_, kEndColor - kStartColor);
}
