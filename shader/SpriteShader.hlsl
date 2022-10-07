//==============================================================================
/// Filename: SpriteShader.hlsl
/// Description: シェーダー
/// Copyright (C)  Silicon Studio Co., Ltd. All rights reserved.
//==============================================================================

Texture2D g_Tex : register(t0); 
SamplerState g_SamplerLinear : register(s0); 

cbuffer ConstantBuffer
{
    float4x4 g_world;
    float4x4 g_view;
    float4x4 g_projection;
}

struct VSOutput
{
    float4 m_pos : SV_Position;
    float4 m_color : COLOR0;
    float2 m_tex : TEXCOORD;
};

VSOutput VS(float4 pos : POSITION,
			    float4 color : COLOR,
				float2 tex : TEXCOORD)
{
    VSOutput Out;
    Out.m_pos = mul(pos,g_world);
    Out.m_pos = mul(Out.m_pos, g_view);
    Out.m_pos = mul(Out.m_pos, g_projection);
    Out.m_color = color;
    Out.m_tex = tex;
    return Out;
}

float4 PS(VSOutput In) : SV_TARGET
{
    return g_Tex.Sample(g_SamplerLinear , In.m_tex);
}