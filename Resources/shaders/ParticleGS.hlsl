#include "Particle.hlsli"
//[maxvertexcount(1)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout PointStream< GSOutput > output
//)
//{
//	GSOutput element;
//
//	element.normal = input[0].normal;
//	element.uv = input[0].uv;
//	element.svpos = input[0].svpos;
//	output.Append(element);
//}
//
//
//[maxvertexcount(3)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout TriangleStream< GSOutput > output
//)
//{
//	//for (uint i = 0; i < 3; i++)
//	//{
//		GSOutput element;
//	//	element.svpos = input[0].svpos;
//		element.normal = input[0].normal;
//		element.uv = input[0].uv;
//
//		element.svpos = input[0].svpos;
//		output.Append(element);
//
//		element.svpos = input[0].svpos + float4(10.0f, 10.0f, 0, 0);
//		output.Append(element);
//
//		element.svpos = input[0].svpos + float4(10.0f, 0, 0, 0);
//
//		output.Append(element);
//	//}
//}

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-1.0f,-1.0f,0,0),
	float4(-1.0f,1.0f,0,0),
	float4(1.0f,-1.0f,0,0),
	float4(1.0f,1.0f,0,0),
};

static const float2 uv_array[vnum] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0)
};

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1]:SV_POSITION,
	inout TriangleStream<GSOutput>output
)
{
	GSOutput element;

	for (uint i = 0; i < vnum; i++) {
		//float4 offset = mul(matBillboard, offset_array[i]);
		float4 offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);
		element.svpos = input[0].pos + offset;
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}