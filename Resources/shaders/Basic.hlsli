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
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float3 normal :NORMAL; // �@���x�N�g��
	//float4 color:COLOR;//�F
	float4 world:POSITION;
	float2 uv  :TEXCOORD; // uv�l
};