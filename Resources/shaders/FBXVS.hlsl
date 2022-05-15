#include"FBX.hlsli"

//スキニング後の頂点・法線が入る
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

//SkinOutput ComputeSkin(VSInput input)
//{
//	SkinOutput output = (SkinOutput)0;
//
//	uint iBone;
//	float weight;
//	matrix m;
//
//	iBone = input.boneIndices.x;
//	weight = input.boneWeights.x;
//	m = matSkinning[iBone];
//	output.pos += weight * mul(m, input.pos);
//	output.normal += weight * mul((float3x3)m, input.normal);
//
//	iBone = input.boneIndices.y;
//	weight = input.boneWeights.y;
//	m = matSkinning[iBone];
//	output.pos += weight * mul(m, input.pos);
//	output.normal += weight * mul((float3x3)m, input.normal);
//
//	iBone = input.boneIndices.z;
//	weight = input.boneWeights.z;
//	m = matSkinning[iBone];
//	output.pos += weight * mul(m, input.pos);
//	output.normal += weight * mul((float3x3)m, input.normal);
//
//	iBone = input.boneIndices.w;
//	weight = input.boneWeights.w;
//	m = matSkinning[iBone];
//	output.pos += weight * mul(m, input.pos);
//	output.normal += weight * mul((float3x3)m, input.normal);
//
//	return output;
//}

SkinOutput ComputeSkin(VSInput input)
{
	SkinOutput output;

	uint iBone;
	matrix m;

	iBone = input.boneIndices.x;
	m = matSkinning[iBone];
	output.pos = mul(m, input.pos);
	output.normal = mul((float3x3)m, input.normal);

	return output;
}

VSOutput main(VSInput input)
{
	//スキニング行列
	SkinOutput skinned = ComputeSkin(input);

	float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));

	VSOutput output;

	output.svpos = mul(mul(viewproj, world), skinned.pos);

	output.normal = wnormal.xyz;

	output.uv = input.uv;

	return output;
}