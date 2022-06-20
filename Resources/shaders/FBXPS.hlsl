#include"FBX.hlsli"

Texture2D<float4> tex:register(t0);

SamplerState smp:register(s0);

struct PSOutput
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;
};

//float4 main(VSOutput input) : SV_TARGET
//{
//	float4 texcolor = tex.Sample(smp,input.uv);
//
//	float3 light = normalize(float3(1, -1, 1));
//	float diffuse = saturate(dot(-light, input.normal));
//	float brightness = diffuse + 0.3f;
//	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
//
//	return shadecolor * texcolor;
//}

PSOutput main(VSOutput input)
{
	PSOutput output;
	float4 texcolor = tex.Sample(smp, input.uv);

	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	output.target0 = shadecolor * texcolor;
	output.target1 = float4(1 - (shadecolor * texcolor).rgb, 1);
	return output;

}