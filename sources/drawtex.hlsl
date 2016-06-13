Texture2D g_texDecal: register(t0);
SamplerState g_samLinear: register(s0);

cbuffer CONSTANT_BUFFER :register(c0)
{
	matrix g_mWP;
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWP);
	output.Tex = Tex;

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	return g_texDecal.Sample(g_samLinear, input.Tex);
}