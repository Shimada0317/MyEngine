#include"Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);


	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);
	output.world = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;

	return output;
}

//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
//{
//	
//
//	float4 wnormal = normalize(mul(world, float4(normal, 0)));
//	float4 wpos = mul(world, pos);
//	float3 ambient = m_ambient;
//	float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;
//
//	const float shininess = 4.0f;
//
//	float3 eyedir = normalize(camerapos - wpos.xyz);
//
//	float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);
//
//	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
//
//	VSOutput output;
//	output.svpos = mul(mul(viewproj, world), pos);
//	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
//	output.color.a = m_alpha;
//	output.uv = uv;
//
//	return output;
//
//}