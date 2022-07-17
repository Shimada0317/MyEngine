#include"toonL.hlsli"


Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);


float4 main(VSOutput input) : SV_TARGET
{
    float3 eyeDir = normalize(camerapos.xyz - input.world);
    float3 halfVec = normalize(_WorldSpaceLightPos0 + eyeDir);
    float intensity = saturate(dot(normalize(input.normal), halfVec));
    fixed4 reflection = pow(intensity, 30);

    float shine = step(0.5, intensity);
    float syadow = 1 - step(0.5, intensity);

    fixed4 l = _Color;
    fixed4 d = _Color * 0.5;
    fixed4 white = (0, 1, 1, 1);


    fixed4 ambient = d * syadow;
    fixed4 diffuse = shine * l;

    fixed4 ads = ambient + diffuse;

    return ads;

}