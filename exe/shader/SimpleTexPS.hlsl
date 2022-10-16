struct VSOutput
{
	float4 Position : SV_POSITION;	// �ʒu���W
	float4 Color	: COLOR0;		// ���_�J���[
	float2 TexCoord	: TEXCOORD0;	// ���_�e�N�X�`��
};

struct PSOutput
{
	float4 Color	: SV_TARGET0;	// �s�N�Z���J���[
};

SamplerState ColorSmp : register(s0);
Texture2D	 ColorMap : register(t0);

// �s�N�Z���V�F�[�_�[�̃��C���G���g���[�|�C���g
PSOutput main(VSOutput Input)
{
	PSOutput output = (PSOutput)0;

	float4 Texcol = ColorMap.Sample(ColorSmp, Input.TexCoord);

	output.Color = Input.Color * Texcol;
	
	return output;
}