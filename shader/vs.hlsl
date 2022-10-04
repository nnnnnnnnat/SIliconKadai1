#include "commonshader.hlsl"

VSOutput main(float4 pos : POSITION , float4 color : COLOR){
	VSOutput Out;

	// ‰½‚à‚¢‚¶‚ç‚È‚¢
	Out.Pos = pos;
	
	return Out;
}