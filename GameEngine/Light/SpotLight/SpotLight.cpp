#include "SpotLight.h"


//inline void SpotLight::LightDirMove(float timex_, float timez_, const XMFLOAT3& startpoint_, const XMFLOAT3& endpoint_)
//{
//	XMVECTOR dir = this->lightdir;
//
//	dir.m128_f32[0] = Action::GetInstance()->EasingOut(timex_, endpoint_.x - startpoint_.x);
//	dir.m128_f32[1] = 0;
//	dir.m128_f32[2] = Action::GetInstance()->EasingOut(timez_, endpoint_.z - startpoint_.z);
//
//	SetLightDir(dir);
//}
