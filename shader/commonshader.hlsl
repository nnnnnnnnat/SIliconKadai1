Texture2D g_Tex : register(t0); // テクスチャ
SamplerState g_SamplerLinear : register(s0); // サンプラー

//--------------------------------------------------------------------------------------
// コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer ConstantBufferWorld : register(b0)
{
    matrix World;
}

cbuffer ConstantBufferView : register(b1)
{
    matrix View;
}

cbuffer ConstantBufferProjection : register(b2)
{
    matrix Projection;
}

cbuffer ConstantBufferMaterial : register(b3)
{
    float4 ambientMaterial;
    float4 diffuseMaterial;
    float4 specularMaterial;
}

cbuffer ConstantBufferLight : register(b4)
{
    float4 LightDirection; // 光の方向
    float4 EyePos; // 視点位置
    float4 Ambient;
}

cbuffer ConstantBufferViewPort : register(b5)
{
    uint4 ViewportWidth; // ビューポート幅
    uint4 ViewportHeight; // ビューポート高さ
};

matrix Identity =
{
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
};

//--------------------------------------------------------------------------------------
// 構造体定義
//--------------------------------------------------------------------------------------
struct VSOutput
{
    float4 m_pos : SV_POSITION;
    float4 m_color : COLOR0;
    float2 m_tex : TEXCOORD;
};

