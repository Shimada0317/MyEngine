#include"PostEffectTest.hlsli"


Texture2D<float4> tex0:register(t0);
Texture2D<float4> tex1:register(t1);
sampler smp:register(s0);

//float4 main(VSOutput input) :SV_TARGET
//{
//	float4 texcolor = tex.Sample(smp,input.uv);
//
//	return float4(texcolor.rgb,1);
//}

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp,input.uv);
	float4 colortex1 = tex1.Sample(smp,input.uv);

	float4 color = colortex0;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = colortex1;
	}


	return float4(color.rgb,1);
}

//float4 main(VSOutput input) : SV_TARGET
//{
//	float4 colortex0 = tex0.Sample(smp,input.uv);
//	float4 colortex1 = tex1.Sample(smp,input.uv);
//
//	float4 color = colortex0;
//
//
//	/*if (fmod(input.uv.y, 0.1f) < 0.05f) {
//	
//		
//		float dy = 1.0f / 720;
//		float dx = 1.0f / 1280;
//		float4 ret = float4(0, 0, 0, 0);
//
//	ret += tex1.Sample(smp, input.uv + float2(-2 * dx, -2 * dy));
//	ret += tex1.Sample(smp, input.uv + float2(0, -2 * dy));
//	ret += tex1.Sample(smp, input.uv + float2(2 * dx, -2 * dy));
//
//	ret += tex1.Sample(smp, input.uv + float2(-2 * dx, 0));
//	ret += tex1.Sample(smp, input.uv);
//	ret += tex1.Sample(smp, input.uv + float2(-2 * dx, 0));
//
//	ret += tex1.Sample(smp, input.uv + float2(-2 * dx, 2 * dy));
//	ret += tex1.Sample(smp, input.uv + float2(0, 2 * dy));
//	ret += tex1.Sample(smp, input.uv + float2(2 * dx, 2 * dy));
//	color = colortex1;
//	return ret / 9;
//	}
//	*/
//	return float4(color.rgb, 1);
//}



