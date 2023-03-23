#include "EasyMath.h"



EasyMath* EasyMath::GetInstance()
{
    static EasyMath instance;

    return &instance;
}

XMVECTOR EasyMath::XMvectorAddXMvector(const XMVECTOR& FirstValue, const XMVECTOR& AddValue)
{
    XMVECTOR valueresult={0.f,0.f,0.f};

    valueresult.m128_f32[0] = FirstValue.m128_f32[0] + AddValue.m128_f32[0];
    valueresult.m128_f32[1] = FirstValue.m128_f32[1] + AddValue.m128_f32[1];
    valueresult.m128_f32[2] = FirstValue.m128_f32[2] + AddValue.m128_f32[2];

    return valueresult;
}

XMVECTOR EasyMath::XMvectorSubXMVector(const XMVECTOR& FirstValue, const XMVECTOR& SubValue)
{
    XMVECTOR valueresult={0.f,0.f,0.f};

    valueresult.m128_f32[0] = FirstValue.m128_f32[0] - SubValue.m128_f32[0];
    valueresult.m128_f32[1] = FirstValue.m128_f32[1] - SubValue.m128_f32[1];
    valueresult.m128_f32[2] = FirstValue.m128_f32[2] - SubValue.m128_f32[2];

    return valueresult;
}

XMFLOAT3 EasyMath::XMFLOAT3AddXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& AddValue)
{
    XMFLOAT3 valueresult = { 0.f,0.f,0.f };

    valueresult.x = FirstValue.x + AddValue.x;
    valueresult.y = FirstValue.y + AddValue.y;
    valueresult.z = FirstValue.z + AddValue.z;

    return valueresult;
}

XMFLOAT3 EasyMath::XMFLOAT3SubXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& SubValue)
{
    XMFLOAT3 valueresult = { 0.f,0.f,0.f };

    valueresult.x = FirstValue.x - SubValue.x;
    valueresult.y = FirstValue.y - SubValue.y;
    valueresult.z = FirstValue.z - SubValue.z;

    return valueresult;
}

XMFLOAT3 EasyMath::XMFLOAT3ChangeValue(const XMFLOAT3& ChangeValue)
{
    XMFLOAT3 valueresult = { 0.f,0.f,0.f };

    valueresult.x = -ChangeValue.x;
    valueresult.y = -ChangeValue.y;
    valueresult.z = -ChangeValue.z;

    return valueresult;
}
