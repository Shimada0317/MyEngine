cbuffer cbuff0:register(b0)
{
	float4 color;
	matrix viewproj;
	matrix world;
	float3 camerapos;
	float timer;
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

static const int POINTLIGHT_NUM = 3;

struct PointLight
{
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	uint active;
};

static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv;    // ライトへの方向の単位ベクトル
	float3 lightcolor;    // ライトの色(RGB)
	uint active;
};

//スポットライトの数
static const int SPOTLIGHT_NUM = 5;

struct SpotLight
{
	float3 lightv;
	float3 lightpos;
	float3 lightcolor;
	float3 lightatten;
	float2 lightfactoranglecos;
	uint active;
};

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
}

