// Gaussian blur vertex shader
// Copyright (C) Wojciech Toman 2008

sampler aoTex: register(s1);

float2 windowSize = {200.0f, 150.0f};
float2 direction = {0.0f, 1.0f};
float blurWidth = 1.0f;
int samplesNum = 7;

float4x4 matWorldViewProj;

const half weights[7] =
{
	0.05,
	0.1,
	0.2,
	0.3,
	0.2,
	0.1,
	0.05
};


struct VertexInput
{
	float3 position : POSITION; 
	float2 texCoord : TEXCOORD0;
};

struct VertexOutput
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};


VertexOutput VS_GaussianBlur(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);
	
	float2 texelSize = blurWidth / windowSize.y;
	float2 s = IN.texCoord - texelSize * (samplesNum - 1) * 0.5f * direction;
	
	OUT.texCoord = IN.texCoord;
	
	return OUT;
}