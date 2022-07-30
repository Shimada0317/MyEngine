cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0); // �A���r�G���g�W��
	float3 m_diffuse  : packoffset(c1); // �f�B�t���[�Y�W��
	float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
	float m_alpha : packoffset(c2.w);	// �A���t�@
}

cbuffer cbuff2:register(b2)
{
	float3 lightv;
	float3 lightcolor;
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	//float3 normal :NORMAL; // �@���x�N�g��
	float4 color:COLOR;
	float2 uv  :TEXCOORD; // uv�l
};