struct VSOutput
{
    float4 m_pos : SV_POSITION;
    float4 m_color : COLOR;
    float2 m_tex : TEXCOORD;
};
float4x4 mtx;

VSOutput main(
    float4 pos : POSITION,
    float4 color : COLOR,
    float4 tex : TEXCOORD
    )
{
    VSOutput vsout = (VSOutput) 0;
    vsout.m_pos = mul(mtx, pos);
    vsout.m_color = color;
    vsout.m_tex = tex;
    return vsout;
}