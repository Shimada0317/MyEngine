#include"Basic.hlsli"


Texture2D<float4>tex:register(t0);
SamplerState smp :register(s0);

struct PSOutput
{
	float4 target0:SV_TARGET0;
};
//
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

//PSOutput main(VSOutput input)
//{
//	PSOutput output;
//	float4 texcolor = tex.Sample(smp, input.uv);
//
//	float3 light = normalize(float3(1, -1, 1));
//	float diffuse = saturate(dot(-light, input.normal));
//	float brightness = diffuse + 0.5f;
//	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
//
//	output.target0 = shadecolor * texcolor*color;
//	return output;
//
//}

//float4 main(VSOutput input) : SV_TARGET
//{
//
//	float4 texcolor = tex.Sample(smp, input.uv);
//	return input.color*texcolor;
//	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);

	float4 shadecolor;

	const float shininess = 4.0f;

	float3 eyedir = normalize(camerapos - input.world.xyz);

	float3 dotlightnormal = dot(lightv, input.normal);

	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);

	float3 ambient = m_ambient;

	float3 diffuse = dotlightnormal * m_diffuse;

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;

	return shadecolor * texcolor*color;
}