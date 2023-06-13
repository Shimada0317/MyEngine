#include "HelperMath.h"
#include"WinApp.h"



HelperMath* HelperMath::GetInstance()
{
    static HelperMath instance;

    return &instance;
}

XMVECTOR HelperMath::XMvectorAddFloat(const XMVECTOR& FirstValue, float AddValue)
{
    XMVECTOR valueresult = { 0.f,0.f,0.f };

    valueresult.m128_f32[0] = FirstValue.m128_f32[0] + AddValue;
    valueresult.m128_f32[1] = FirstValue.m128_f32[1] + AddValue;
    valueresult.m128_f32[2] = FirstValue.m128_f32[2] + AddValue;

    return valueresult;
}

XMVECTOR HelperMath::XMvectorAddXMvector(const XMVECTOR& FirstValue, const XMVECTOR& AddValue)
{
    XMVECTOR valueresult={0.f,0.f,0.f};

    valueresult.m128_f32[0] = FirstValue.m128_f32[0] + AddValue.m128_f32[0];
    valueresult.m128_f32[1] = FirstValue.m128_f32[1] + AddValue.m128_f32[1];
    valueresult.m128_f32[2] = FirstValue.m128_f32[2] + AddValue.m128_f32[2];

    return valueresult;
}

XMVECTOR HelperMath::XMvectorSubXMVector(const XMVECTOR& FirstValue, const XMVECTOR& SubValue)
{
    XMVECTOR valueresult={0.f,0.f,0.f};

    valueresult.m128_f32[0] = FirstValue.m128_f32[0] - SubValue.m128_f32[0];
    valueresult.m128_f32[1] = FirstValue.m128_f32[1] - SubValue.m128_f32[1];
    valueresult.m128_f32[2] = FirstValue.m128_f32[2] - SubValue.m128_f32[2];

    return valueresult;
}

XMFLOAT3 HelperMath::XMFLOAT3AddFloat(const XMFLOAT3& FirstValue, float AddValue)
{
    XMFLOAT3 valueresult = { 0.f,0.f,0.f };

    valueresult.x = FirstValue.x + AddValue;
    valueresult.y = FirstValue.y + AddValue;
    valueresult.z = FirstValue.z + AddValue;

    return valueresult;
}

XMFLOAT3 HelperMath::XMFLOAT3AddXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& AddValue)
{
    XMFLOAT3 valueresult = { 0.f,0.f,0.f };

    valueresult.x = FirstValue.x + AddValue.x;
    valueresult.y = FirstValue.y + AddValue.y;
    valueresult.z = FirstValue.z + AddValue.z;

    return valueresult;
}

XMFLOAT3 HelperMath::XMFLOAT3SubXMFLOAT3(const XMFLOAT3& FirstValue, const XMFLOAT3& SubValue)
{
    XMFLOAT3 valueresult = { 0.f,0.f,0.f };

    valueresult.x = FirstValue.x - SubValue.x;
    valueresult.y = FirstValue.y - SubValue.y;
    valueresult.z = FirstValue.z - SubValue.z;

    return valueresult;
}

XMFLOAT3 HelperMath::XMFLOAT3ChangeValue(const XMFLOAT3& ChangeValue)
{
    XMFLOAT3 valueresult = { 0.f,0.f,0.f };

    valueresult.x = -ChangeValue.x;
    valueresult.y = -ChangeValue.y;
    valueresult.z = -ChangeValue.z;

    return valueresult;
}

XMFLOAT3 HelperMath::ConvertToXMFLOAT3(float SourceValue)
{
    XMFLOAT3 ConvertValue;

    ConvertValue.x = SourceValue;
    ConvertValue.y = SourceValue;
    ConvertValue.z = SourceValue;

    return ConvertValue;
}

XMFLOAT3 HelperMath::ConvertToXMVECTOR(XMVECTOR value)
{
    XMFLOAT3 Value;
    Value.x = value.m128_f32[0];
    Value.y = value.m128_f32[1];
    Value.z = value.m128_f32[2];

    return Value;
}

XMFLOAT3 HelperMath::SquareToXMFLOAT3(XMFLOAT3 value, float squarevalue)
{
    XMFLOAT3 Value;

    Value.x = powf(value.x, squarevalue);
    Value.y = powf(value.y, squarevalue);
    Value.z = powf(value.z, squarevalue);

    return Value;
}

float HelperMath::LengthCalculation(XMFLOAT3 value)
{
    float Length;

    Length = sqrtf(value.x + value.y + value.z);

    return Length;
}

void HelperMath::ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset)
{
    matviewport.r[0].m128_f32[0] = WinApp::window_width / 2;
    matviewport.r[0].m128_f32[1] = 0;
    matviewport.r[0].m128_f32[2] = 0;
    matviewport.r[0].m128_f32[3] = 0;

    matviewport.r[1].m128_f32[0] = 0;
    matviewport.r[1].m128_f32[1] = -(WinApp::window_height / 2);
    matviewport.r[1].m128_f32[2] = 0;
    matviewport.r[1].m128_f32[3] = 0;

    matviewport.r[2].m128_f32[0] = 0;
    matviewport.r[2].m128_f32[1] = 0;
    matviewport.r[2].m128_f32[2] = 1;
    matviewport.r[2].m128_f32[3] = 0;

    matviewport.r[3].m128_f32[0] = WinApp::window_width / 2 + offset.m128_f32[0];
    matviewport.r[3].m128_f32[1] = WinApp::window_height / 2 + offset.m128_f32[1];
    matviewport.r[3].m128_f32[2] = 0;
    matviewport.r[3].m128_f32[3] = 1;
}

XMFLOAT3 HelperMath::TrackCalculation(const XMVECTOR& position, const XMVECTOR& trackpoint)
{
    XMFLOAT3 Value;

    Value.x = position.m128_f32[0] - trackpoint.m128_f32[0];
    Value.y = position.m128_f32[1] - trackpoint.m128_f32[1];
    Value.z = position.m128_f32[2] - trackpoint.m128_f32[2];

    return Value;
}

XMVECTOR HelperMath::TrackingVelocityCalculation(const XMFLOAT3& value, float length, float speed)
{
    XMVECTOR Value{};
    Value.m128_f32[0] = (value.x / length) * speed;
    Value.m128_f32[1] = (value.y / length) * speed;
    Value.m128_f32[2] = (value.z / length) * speed;
    return Value;
}

void HelperMath::TrackEnemytoPlayer(XMVECTOR& position,const XMVECTOR trackspeed)
{
    position.m128_f32[0] -= trackspeed.m128_f32[0];
    position.m128_f32[2] -= trackspeed.m128_f32[2];
}
