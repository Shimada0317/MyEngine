//cbuffer cbuff0 : register(b0)
//{
//	matrix mat; // ３Ｄ変換行列
//};
//
//cbuffer cbuff1 : register(b1)
//{
//	float3 m_ambient  : packoffset(c0); // アンビエント係数
//	float3 m_diffuse  : packoffset(c1); // ディフューズ係数
//	float3 m_specular : packoffset(c2); // スペキュラー係数
//	float m_alpha : packoffset(c2.w);	// アルファ
//}
//
//
//struct VSOutput
//{
//	float2 uv : TEXCOORD0;
//	float4 svpos : SV_POSITION;
//};