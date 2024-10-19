#include "Lambert.hlsli"

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);

float4 main(VS_OUT pin) : SV_TARGET
{
	//ハイトマップから高さを取得
	float height = Height_map.Sample(diffuseMapSamplerState,pin.texcoord).r;
	//頂点位置の変位
	float3 displacedPosition = pin.world_position + float3(0.0, height * 1.0f, 0.0);
	pin.position += float4(displacedPosition, 1.0);
	//法線マップからxyz成分を取得して（-1 ~ +1）の間にスケーリング
	float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).xyz * 2 - 1;

	//カメラ行列から視線行列の抜き取り
	float3 viewDir;
	viewDir.x = viewProjection[3][1];
	viewDir.y = viewProjection[3][2];
	viewDir.z = viewProjection[3][3];

	viewDir = normalize(viewDir);

	//変換用の3X3行列を用意する
	float3x3 CM = {
		normalize(pin.tangent),
		normalize(pin.binormal),
		normalize(pin.normal),
	};

	//マテリアル定数
	//float3 ka = float3(1, 1, 1);
	//float3 kd = float3(1, 1, 1);
	//float3 ks = float3(1, 1, 1);
	float shiness = 512;

	float3 N = normalize(mul(normal, CM));
	float3 L = normalize(lightDirection.xyz);
	float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);

	//環境光の計算
	//float3 ambient = ka * ambientLightColor;
	float3 ambient = stageColor;

	//視差効果の適応
	float2 correctedUV = ParallaxMapping(pin.texcoord, viewDir, 0.05, -0.02);
	float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, correctedUV) * pin.color;

	//平行光源のライティング計算
	float3 directionalSpecular = CalcPhongSpecular(N, L, lightColor.rgb, E, shiness, diffuseColor.rgb);


	float4 color = float4(ambient, diffuseColor.a);
	color.rgb += diffuseColor.rgb;
	color.rgb += directionalSpecular;
	return color;
}