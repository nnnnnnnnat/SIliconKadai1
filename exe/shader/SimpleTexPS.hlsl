struct VSOutput
{
	float4 Position : SV_POSITION;	// 位置座標
	float4 Color	: COLOR0;		// 頂点カラー
	float2 TexCoord	: TEXCOORD0;	// 頂点テクスチャ
};

struct PSOutput
{
	float4 Color	: SV_TARGET0;	// ピクセルカラー
};

SamplerState ColorSmp : register(s0);
Texture2D	 ColorMap : register(t0);

// ピクセルシェーダーのメインエントリーポイント
PSOutput main(VSOutput Input)
{
	PSOutput output = (PSOutput)0;

	float4 Texcol = ColorMap.Sample(ColorSmp, Input.TexCoord);

	output.Color = Input.Color * Texcol;
	
	return output;
}