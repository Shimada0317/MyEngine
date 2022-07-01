#include"PostEffectTest.hlsli"

Texture2D<float4> tex:register(t0);
sampler smp:register(s0);

float4 main(VSOutput input) :SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);

	return float4(texcolor.rgb,1);

	//float dy = 1.0f / 720;
	//float dx = 1.0f / 1280;
	//float4 ret = float4(0, 0, 0, 0);

	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy));
	//ret += tex.Sample(smp, input.uv + float2(0, -2 * dy));
	//ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy));

	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0));
	//ret += tex.Sample(smp, input.uv);
	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0));

	//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy));
	//ret += tex.Sample(smp, input.uv + float2(0, 2 * dy));
	//ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy));

	//return ret/9;
}