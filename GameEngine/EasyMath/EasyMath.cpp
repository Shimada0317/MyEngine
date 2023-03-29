#include "EasyMath.h"
#include"WinApp.h"



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

XMFLOAT3 EasyMath::ConvertToXMFLOAT3(float SourceValue)
{
    XMFLOAT3 ConvertValue;

    ConvertValue.x = SourceValue;
    ConvertValue.y = SourceValue;
    ConvertValue.z = SourceValue;

    return ConvertValue;
}

void EasyMath::ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset)
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
