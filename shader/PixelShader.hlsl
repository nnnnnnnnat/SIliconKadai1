Texture2D g_tex : register(t0);
SamplerState g_sampler : register(s0);

struct VSOutput
{
    float4 m_pos : SV_POSITION;
    float4 m_color : COLOR;
    float2 m_tex : TEXCOORD;
};

float4 main(VSOutput vsout) : SV_TARGET
{
    return vsout.m_color;
    //return g_sampler.Sample(g_tex, vsout.Tex);

}