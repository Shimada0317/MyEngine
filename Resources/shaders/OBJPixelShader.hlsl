#include "OBJShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp,input.uv);

	

	const float shininess = 4.0f;

	float3 eyedir = normalize(camerapos - input.world.xyz);

	/*float3 dotlightnormal = dot(lightv, input.normal);

	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);*/

	float3 ambient = m_ambient;

	//float3 diffuse = dotlightnormal * m_diffuse;

	//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	for (int i = 0; i < POINTLIGHT_NUM; i++) {
		if (pointLights[i].active) {
			//ライトへのベクトル
			float3 lightv = pointLights[i].lightpos - input.world.xyz;
			//ベクトルの長さ
			float d = length(lightv);
			//正規化し、単位ベクトルにする
			lightv = normalize(lightv);
			//距離減衰係数
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
				pointLights[i].lightatten.z * d * d);
			//ライトンに向かうベクトルの法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}

	//スポットライト
	for (int j = 0; j < SPOTLIGHT_NUM; j++) {
		if (spotLights[j].active) {
			//ライトへの方向ベクトル
			float3 lightv = spotLights[j].lightpos - input.world.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			//距離減衰係数
			float atten = saturate(1.0f / (spotLights[j].lightatten.x + spotLights[j].lightatten.y * d + spotLights[j].lightatten.z * d*d));
			//角度減衰
			float cos = dot(lightv, spotLights[j].lightv);
			//減衰開始角度から、減衰終了角度にかけて減衰
			//減衰開始角度の内側は1倍減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(spotLights[j].lightfactoranglecos.y, spotLights[j].lightfactoranglecos.x, cos);
			//角度減衰を乗算
			atten *= angleatten;
			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[j].lightcolor;
		}
	}
	return shadecolor * texcolor * color;
}