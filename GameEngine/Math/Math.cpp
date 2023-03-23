#include "Math.h"



Math* Math::GetInstance()
{
    static Math instance;

    return &instance;
}

XMVECTOR Math::XMvectorAddXMvector(const XMVECTOR& FirstValue, const XMVECTOR& AddValue)
{
    XMVECTOR result;

    result.m128_f32[0] = FirstValue.m128_f32[0] + AddValue.m128_f32[0];
    result.m128_f32[1] = FirstValue.m128_f32[1] + AddValue.m128_f32[1];
    result.m128_f32[2] = FirstValue.m128_f32[2] + AddValue.m128_f32[2];

    return result;
}

XMVECTOR Math::XMvectorSubXMVector(const XMVECTOR& FirstValue, const XMVECTOR& SubValue)
{
    XMVECTOR result;

    result.m128_f32[0] = FirstValue.m128_f32[0] - SubValue.m128_f32[0];
    result.m128_f32[1] = FirstValue.m128_f32[1] - SubValue.m128_f32[1];
    result.m128_f32[2] = FirstValue.m128_f32[2] - SubValue.m128_f32[2];

    return result;
}

XMFLOAT3 Math::XMFLOAT3AddXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& AddValue)
{
    XMFLOAT3 result;

    result.x = FirstValue.x + AddValue.x;
    result.y = FirstValue.y + AddValue.y;
    result.z = FirstValue.z + AddValue.z;

    return result;
}

XMFLOAT3 Math::XMFLOAT3SubXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& SubValue)
{
    XMFLOAT3 result;

    result.x = FirstValue.x - SubValue.x;
    result.y = FirstValue.y - SubValue.y;
    result.z = FirstValue.z - SubValue.z;

    return result;
}

XMFLOAT3 Math::XMFLOAT3ChangeValue(const XMFLOAT3& ChangeValue)
{
    XMFLOAT3 result;

    result.x = -ChangeValue.x;
    result.y = -ChangeValue.y;
    result.z = -ChangeValue.z;

    return result;
}
