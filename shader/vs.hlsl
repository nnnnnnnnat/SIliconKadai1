#include "commonshader.hlsl"

VSOutput main(float4 Pos : POSITION,
				float2 Tex : TEXCOORD,
			    float4 Color : COLOR)
{
    
    VSOutput output = (VSOutput) 0;
    output.m_pos = Pos;
    //output.m_pos = mul(Pos, World);
    //output.m_wPos = output.m_pos;
    //output.m_pos = mul(output.m_pos, View);
    //output.m_pos = mul(output.m_pos, Projection);
    output.m_tex = Tex;
        
    output.m_color = Color;
    
    return output;

}