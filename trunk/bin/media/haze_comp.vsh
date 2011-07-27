// Haze composition vertex shader
// Copyright (C) Wojciech Toman 2009

struct VertexInput
{
	float3 position: POSITION0;
	float4 diffuse:  COLOR0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 position: POSITION0;
	float2 texCoord: TEXCOORD0;
	float3 lightDir: TEXCOORD1;
};

float3 lightDir;

float4x4 matView;
float4x4 matWorldViewProj;


VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);
	OUT.texCoord = IN.texCoord;
	OUT.lightDir = -(mul(float4(lightDir, 0.0f), matView).xyz);

	return OUT;
}