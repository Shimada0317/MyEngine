#include"Basic.hlsli"


Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);

struct PSOutput
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;
};

//float4 main(VSOutput input) : SV_TARGET
//{
//	float3 light = normalize(float3(1,-1,1));
//	float3 light_diffuse = saturate(dot(-light, input.normal));
//	float3 shade_color;
//	shade_color = m_ambient;
//	shade_color += m_diffuse * light_diffuse;
//	float4 texcolor = tex.Sample(smp, input.uv);
//	return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
//	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
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