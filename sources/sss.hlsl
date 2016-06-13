cbuffer GLOBAL
{
	matrix g_mW: packoffset(c0);	//c(n) n��16�o�C�g�P��	//���[���h�s��
	matrix g_mWVP: packoffset(c4);	//���[���h����ˉe�܂ł̕ϊ��s��
	matrix g_mWLPT: packoffset(c8);
	matrix g_mWLP: packoffset(c12);
	float3 g_vLightPos: packoffset(c16);	//���C�g�̕����x�N�g��
	//float4 g_Ambient: packoffset(c17);	//�A���r�G���g��
	float3 g_Scoeff: packoffset(c17); //�U���W��
	float4 g_Diffuse: packoffset(c18);	//�g�U����
	float4 g_Specular: packoffset(c19);	//���ʔ���
	float3 g_vEye: packoffset(c20);
	float g_Transparent : packoffset(c21);	//�ǂ��܂ł̒����Ȃ瓧�߂��邩
	float g_HalfThicknessPixel : packoffset(c22);
	float g_DepthPixel : packoffset(c23);
	float g_HalfDepthPixel : packoffset(c24);
	float2 g_random[256] : packoffset(c25);
	
};

Texture2D g_texDepth: register(t0);
Texture2D g_texThickness: register(t1);
Texture2D g_texNaKaMi: register(t2);
//Texture2D g_texIrradiance: register(t3);
Texture2D g_texNormal: register(t3);
Texture2D g_texFresnel: register(t4);
Texture2D g_texPosition: register(t5);

SamplerState g_samPoint: register(s0);
SamplerState g_samLinear: register(s1);


//�\����
struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};
//�\����
struct VS_OUT2
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
};
//�\����
struct VS_OUT3
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD0;
	float3 PosWorldspace : POSITION;
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float4 transparent : TEXCOORD3;
};

//��ԏC���q�̎��
//linear
//centroid
//nointerpolation
//noperspective
//sample

//
//�[�x�}�b�v�쐬�@�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUT VS_Depth(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUT output = (VS_OUT)0;
	output.Pos = mul(Pos, g_mWLP);
	//���C�g�r���[����̃e�N�X�`�����W���݂�
	output.Tex = Tex;
	return output;
}
//
//�[�x�}�b�v�쐬�@�s�N�Z���V�F�[�_�[
//
float4 PS_Depth(VS_OUT input) : SV_Target
{
	//return float4(input.Pos.w, 0, 0, 1);
	return float4(input.Tex.x, 0, 0, 1);//R32�t�H�[�}�b�g�Ȃ̂ŐԈȊO�͕s�g�p
}

////
////���ˏƓx�}�b�v�@VS
////
//VS_OUT2 VS_Irradiance(float4 Pos : POSITION, float4 Normal : NORMAL)
//{
//	VS_OUT2 output = (VS_OUT2)0;
//	output.Pos = mul(Pos, g_mWLP);
//	output.Normal = mul(Normal, (float3x3)g_mW);
//
//	//point light
//	//float3 PosWorld = mul(Pos, g_mW);
//	//output.Light = g_vLightPos - PosWorld;
//
//	//parallel light
//	output.Light = g_vLightPos.xyz;
//
//	return output;
//}
////
////���ˏƓx�}�b�v�@PS
////
//float4 PS_Irradiance(VS_OUT2 input) : SV_Target
//{
//	float3 Normal = normalize(input.Normal);
//	float3 LightVector = normalize(input.Light);
//	float NdotL = max(0.0, dot(Normal, LightVector));
//
//	//Fresnel Refraction(F)
//	float k = pow(1.0 - NdotL, 5.0);
//	float n = 1.33;
//	float A = (1 - n) / (1 + n);
//	float F0 = pow(A, 2);
//	float Ft = (1.0 - F0)*k + F0;
//
//	//light irradiance
//	float Irradiance = NdotL;// / (input.Pos.w * input.Pos.w);
//
//	//Incident Light at surface point
//	float E = (1.0-Ft) * NdotL * Irradiance * 1.0;
//	//float E = (1.1-NdotL)*4.0; //
//
//	return float4(E, 0, 0, 1);
//}

//
//�@���}�b�v�@VS
//
VS_OUT3 VS_Normal(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUT3 output = (VS_OUT3)0;
	output.Pos = mul(Pos, g_mWLP);
	output.Normal = mul(Normal, (float3x3)g_mW);

	return output;
}
//
//�@���}�b�v�@PS
//
float4 PS_Normal(VS_OUT3 input) : SV_Target
{
	float3 Normal = normalize(input.Normal);	//-1~1

	return float4((Normal + 1) / 2.0, 1.0);	//0~1
}


//
//Fresnel Reflaction �}�b�v�@VS
//
VS_OUT2 VS_Fresnel(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUT2 output = (VS_OUT2)0;
	output.Pos = mul(Pos, g_mWLP);
	output.Normal = mul(Normal, (float3x3)g_mW);
	output.Light = g_vLightPos;		//parallel right

	return output;
}
//
//Fresnel Reflaction �}�b�v PS
//
float4 PS_Fresnel(VS_OUT2 input) : SV_Target
{
	float3 IncidentVec = - normalize(input.Light); 
	float3 Normal = normalize(input.Normal);
	//float n = 1.33;
	//float cosA = dot(Normal, -IncidentVec);
	//float cosB = 1.0/1.33 * sqrt(n*n - (1 - cosA*cosA));
	//float3 ReflactVec = 1.0/1.33 * (IncidentVec - (sqrt(n*n - (1 - cosA*cosA)) - cosA)*Normal); //-1~1

	float etaO = 1.0f;
	float etaI = 1.333f;
	float nnt = etaO / etaI;
	float ddn = dot(IncidentVec, Normal);
	float cos2t = 1.0f - nnt * nnt * (1.0f - ddn * ddn);

	if (cos2t < 0.0) {

	}

	//float3 ReflactVec = normalize(1.0 / 1.33 * (IncidentVecfromView - (sqrt(n*n - (1 - cosA*cosA)) - cosA)* output.Normal)); //-1~1
	float3 ReflactVec = normalize(IncidentVec* nnt - Normal * 1.0 * (ddn * nnt + sqrt(cos2t))); //-1~1


	return float4((ReflactVec + 1) / 2.0, 1.0);	//0~1
}

//
//Position Map VS
//
VS_OUT3 VS_Position(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUT3 output = (VS_OUT3)0;
	output.Pos = mul(Pos, g_mWLP);
	output.Normal = mul(Normal, (float3x3)g_mW);
	output.PosWorldspace = mul(Pos, g_mW);
	
	return output;
}
//
//Position Map PS
//
float4 PS_Position(VS_OUT3 input) : SV_Target
{
	float3 Position = input.PosWorldspace;
	Position = (Position + 1.0) / 2.0;
	return float4(Position, 1.0);
}


//
//SSS�@�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Normal, (float3x3)g_mW);
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;
	
	float g_HalfDepthPixel = g_DepthPixel * 0.5f;	
	
	//point light
	//output.Light = normalize(g_vLightPos - PosWorld);

	//parallel light
	output.Light = normalize(g_vLightPos);

	//light in (Voxel Number)
	float4 texcoord = mul(Pos, g_mWLPT);	//���̓_�����C�g�r���[�ł������Ƃ��̈ʒu
	texcoord.xyz /= texcoord.w;

	//float LightIn = 0;
	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + g_HalfDepthPixel), 0).r; //number

	//light out (Voxel Number)
	float LightOut = Tex.x; //number

	//�U���W��
	float3 s = g_Scoeff;
	float3 s2 = float3(0.1627, 0.8156, 0.898); //float3(0.9, 0.9, 0.02);//float3(1.0, 0.85, 0.18);// float3(0.766, 0.445, 0.058);

	//�`��_����̃t���l�����ߕ���
	float3 IncidentVecfromView = -normalize(output.EyeVector);
	float n = 1.33;
	float cosA = dot(output.Normal, -IncidentVecfromView);
	//float cosB = 1.0/1.33 * sqrt(n*n - (1 - cosA*cosA));


	float etaO = 1.0f;
	float etaI = 1.333f;
	float nnt = etaO / etaI;
	float ddn = dot(IncidentVecfromView, output.Normal);
	float cos2t  = 1.0f - nnt * nnt * (1.0f - ddn * ddn);

	if (cos2t < 0.0) {
		
	}

	//float3 ReflactVec = normalize(1.0 / 1.33 * (IncidentVecfromView - (sqrt(n*n - (1 - cosA*cosA)) - cosA)* output.Normal)); //-1~1
	float3 ReflactVec = normalize(IncidentVecfromView * nnt - output.Normal * 1.0 * (ddn * nnt + sqrt(cos2t))); //-1~1

	//return float4((ReflactVec + 1) / 2.0, 1.0);	//0~1

	//�g�U�ߎ�
	float k1 = 0.05556; // 1/18
	float k2 = 0.06250; // 1/16
	float k3 = 0.04167; // 1/24
	float k4 = 0.02083; // 1/48

	output.transparent = float4(0, 0, 0, 1);

	float sum_weight = 0;

	//���߂����	
	for (int i = 0; i < 256; i++) {

		float2 random = texcoord.xy;//  +(g_random[i].xy + 1) * 0.5f;
		
		float LightIn = g_texDepth.SampleLevel(g_samPoint, float2(random + g_HalfDepthPixel), 0).r; //number

		float3 Ps = g_texPosition.SampleLevel(g_samPoint, float2(random + g_HalfDepthPixel), 0).xyz;
		Ps = 2 * Ps - 1;

		float3 Ns = g_texNormal.SampleLevel(g_samPoint, float2(random + g_HalfDepthPixel), 0).xyz;
		Ns = 2 * Ns -  1;

		float3 ds = g_texFresnel.SampleLevel(g_samPoint, float2(random + g_HalfDepthPixel), 0).xyz;
		ds = 2 * ds - 1;


		if (LightIn != 0) {
			//thickness
			float thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;

			//NaKaMi!!
			float nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;

			//nakami = 0;
			thickness = thickness - nakami;

			//Irradiance
			//float E = g_texIrradiance.SampleLevel(g_samLinear, float2(texcoord.xy + g_HalfDepthPixel), 0).r;


			float A = max(0.0f, dot(ds, (PosWorld - Ps)));
			float B = max(0.0f, dot(ReflactVec, (Ps - PosWorld)));
			float C = pow(distance(PosWorld, Ps), 4.0f);

			float con = A * B / (C + 0.00001);

			sum_weight += con;

			output.transparent += con * float4(exp(-(s * thickness + s2 * nakami) * g_Transparent), 1);
		}
	}

	output.transparent /= sum_weight;

	//output.transparent = k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami) * g_Transparent), 1);
	
	////1.0
	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(-1, 0) * g_DepthPixel + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(0, -1) * g_DepthPixel + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(1, 0) * g_DepthPixel + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(0, 1) * g_DepthPixel + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	////2.0
	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(-1, 0) * g_DepthPixel * 2 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(0, -1) * g_DepthPixel * 2 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(1, 0) * g_DepthPixel * 2 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(0, 1) * g_DepthPixel * 2 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k1 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	////1.5
	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(-1, 1) * g_DepthPixel * 1.5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k2 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(-1, -1) * g_DepthPixel * 1.5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k2 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(1, -1) * g_DepthPixel * 1.5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k2 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(1, 1) * g_DepthPixel * 1.5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k2 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	////4
	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(-1, 1) * g_DepthPixel * 4 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k3 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(-1, -1) * g_DepthPixel * 4 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k3 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(1, -1) * g_DepthPixel * 4 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k3 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(1, 1) * g_DepthPixel * 4 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k3 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	////5
	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(-1, 0) * g_DepthPixel * 5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k4 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(0, -1) * g_DepthPixel * 5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k4 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(1, 0) * g_DepthPixel * 5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k4 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	//LightIn = g_texDepth.SampleLevel(g_samPoint, float2(texcoord.xy + float2(0, 1) * g_DepthPixel * 5 + g_HalfDepthPixel), 0).r;
	//thickness = g_texThickness.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	//nakami = g_texNaKaMi.SampleLevel(g_samPoint, float2(LightOut + g_HalfThicknessPixel, LightIn + g_HalfThicknessPixel), 0).r;
	////nakami = 0;
	//thickness = thickness - nakami;
	//output.transparent += k4 *  g_Diffuse * float4(exp(-(s * thickness + s2 * nakami)* g_Transparent), 1);

	output.transparent.w = 1;

	return output;
}

//
//SSS�@�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = float4(0, 0, 0, 1);

	float3 Normal = normalize(input.Normal);
	float3 LightVector = normalize(input.Light);
	float3 EyeVector = normalize(input.EyeVector);
	float NdotL = dot(Normal, LightVector);

	//Fresnel Refraction(F)
	float NdotL2 = max(0,dot(-Normal, LightVector));
	float NdotL3 = max(0,dot(Normal, EyeVector));
	float n = 1.33;//0.71428;

	//float cos = sqrt(1 - ((1 - pow(NdotL2, 2.0)) / pow(n, 2.0)));
	//float B = sqrt(1 - pow(n, 2.0)*(1 - pow(dot(EyeVector, Normal), 2.0)));
	float k = pow(1.0 - NdotL3, 5.0);
	//float k = pow(1.0 - cos, 5.0);
	float A = (1 - n) / (1 + n);
	float F0 = pow(A, 2);
	float Ft = (1.0 - F0)*k + F0;

	//�t�H���V�F�[�f�B���O
	/*float3 Reflect = normalize(2 * NdotL*Normal - LightVector);
	float specular = pow(saturate(dot(Reflect, EyeVector)), 8);
	float4 phong = g_Diffuse * saturate(NdotL) + 4 * specular*g_Specular;*/

	color.xyz = (1.0 - Ft) * input.transparent.xyz * 1.0;

	//color.xyz = input.transparent.xyz * 1.5;
	
	//color = 2*phong/4.0f+2*input.transparent*2.0f/4.0f;//�t�H���V�F�[�f�B���O�̐F�ɓ��ߌ����v���X

	//�����o�[�g
	if (NdotL>0)
	{
		float3 h = normalize(LightVector + EyeVector);
		color += pow(max(dot(h, Normal), 0.0), 50);
	}


	return float4(color.xyz,1);

	//return float4(0,0,Normal.z, 1.0); //�m�F�p�Ƃ��ɂǂ���
}