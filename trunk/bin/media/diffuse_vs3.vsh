// Diffuse vertex shader
// Copyright (C) Wojciech Toman 2008

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
};

float4x4 matWorldViewProj;
float4x4 matWorld;

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);
	OUT.texCoord = IN.texCoord;

	return OUT;
}