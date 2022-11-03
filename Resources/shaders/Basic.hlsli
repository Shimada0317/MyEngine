cbuffer cbuff0:register(b0)
{
	float4 color;
	matrix viewproj;
	matrix world;
	float3 camerapos;

};

cbuffer cbuff1:register(b1)
{
	float3 m_ambient:packoffset(c0);
	float3 m_diffuse:packoffset(c1);
	float3 m_specular:packoffset(c2);
	float m_alpha : packoffset(c2.w);
}

struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal :NORMAL; // 法線ベクトル
	//float4 color:COLOR;//色
	float4 world:POSITION;
	float2 uv  :TEXCOORD; // uv値
};