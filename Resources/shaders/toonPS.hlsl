#include"toonL.hlsli"


Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);


float4 main(VSOutput input) : SV_TARGET
{

    float4 color = {1,0,1,1};

    float3 white = { 1, 1, 1};

    float3 eyeDir = normalize(camerapos.xyz - input.world);
    float3 halfVec = normalize(float3(0,1,0) + eyeDir);
    float intensity = saturate(dot(normalize(input.normal), halfVec));
    float4 reflection = pow(intensity, 30);

    float shine = step(0.3, intensity);
    float syadow = 1 - step(0.5, intensity);

    float3 l = color.rgb;
    float3 d = color.rgb*0.5f;

    float3 ambient = d * syadow;
    float3 diffuse = shine * l;
    float3 specular = step(0.5, reflection) * white;
    

    return float4(ambient + specular + diffuse, 1);

}