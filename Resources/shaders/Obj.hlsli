//cbuffer cbuff0 : register(b0)
//{
//	matrix mat; // �R�c�ϊ��s��
//};
//
//cbuffer cbuff1 : register(b1)
//{
//	float3 m_ambient  : packoffset(c0); // �A���r�G���g�W��
//	float3 m_diffuse  : packoffset(c1); // �f�B�t���[�Y�W��
//	float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
//	float m_alpha : packoffset(c2.w);	// �A���t�@
//}
//
//
//struct VSOutput
//{
//	float2 uv : TEXCOORD0;
//	float4 svpos : SV_POSITION;
//};