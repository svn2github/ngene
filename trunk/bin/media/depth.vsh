// Light-position vertex shader
// Copyright (C) Wojciech Toman 2007

float4x4 matWorldViewProj;
float4x4 matWorld;

struct VertexInput
{
	float3 position: POSITION0;
};

struct VertexOutput
{
	float4 position: POSITION0;
	float3 pos: TEXCOORD1;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);
	OUT.pos = mul(float4(IN.position, 1.0f), matWorld);

	return OUT;
}