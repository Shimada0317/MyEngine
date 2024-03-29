#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);


	VSOutput output;
	output.svpos = mul(mul(viewproj, world), pos);
	output.world = wpos;
	output.normal = wnormal.xyz;
	output.uv.x = uv.x+timer;
	output.uv.y = uv.y;

	return output;
}