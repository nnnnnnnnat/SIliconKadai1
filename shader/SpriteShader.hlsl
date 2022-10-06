Texture2D g_Tex : register(t0); 
SamplerState g_SamplerLinear : register(s0); 

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
    Out.m_pos = pos;
    Out.m_color = color;
    Out.m_tex = tex;
    return Out;
}

float4 PS(VSOutput In) : SV_TARGET
{
    return g_Tex.Sample(g_SamplerLinear , In.m_tex);
    //return float4(0.0f, 0.0f, 1.0f, 1.0f);
}