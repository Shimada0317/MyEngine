#include"toonL.hlsli"


Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);


float4 main(VSOutput input) : SV_TARGET
{

    float4 color = tex.Sample(smp,input.uv);

    float3 eyeDir = normalize(camerapos.xyz - input.world);
    float3 halfVec = normalize(float3(0,0,0) + eyeDir);
    float intensity = saturate(dot(normalize(input.normal), halfVec));
    float4 reflection = pow(intensity, 30);

    float shine = step(0.3, intensity);
    float syadow = 1 - step(0.5, intensity);

    float4 l = color;
    float4 d = (color.r * 0.5f, color.g * 0.5f, color.b * 0.5f, color.a*0.5);
    float4 white = (0, 0, 0, 1);


    float4 ambient = d * syadow;
    float4 diffuse = shine * l;
    float4 specular = step(0.5, reflection) * white;
    float4 ads = ambient + diffuse + specular;

    return ads;

}