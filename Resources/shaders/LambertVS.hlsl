#include "Lambert.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	

	//ライトの距離
	float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);
	//ライトの色
	float3 lightcolor = float3(1, 1, 1);

	//ピクセルシェーダに渡す値
	VSOutput output;
	output.svpos = mul(mat, pos);

	//Lambert反射の計算
	output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;

	float3 ambient = m_ambient;
	float3 diffuse = dot(-lightdir, normal) * m_diffuse;

	const float3 eye = float3(0, 0, -20);

	const float shininess = 4.0f;

	float3 eyedir = normalize(eye - pos.xyz);

	float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);

	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
	output.color.a = m_alpha;

	
	return output;
}