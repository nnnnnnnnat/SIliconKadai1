#include "commonshader.hlsl"

VSOutput main(float4 pos : POSITION , float4 color : COLOR){
	VSOutput Out;

	// ����������Ȃ�
	Out.Pos = pos;
	
	return Out;
}