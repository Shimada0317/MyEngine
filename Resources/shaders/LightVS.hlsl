#include "Light.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{


	float4 wnormal = normalize(mul(world, float4 (normal, 0)));

	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);

	//Lambert���˂̌v�Z
	output.color.a = m_alpha;
	output.uv = uv;

	float3 ambient = m_ambient;
	float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;

	const float shininess = 4.0f;

	float3 eyedir = normalize(cameraPos - pos.xyz);

	float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
	output.color.a = m_alpha;


	return output;
}