// Light-directional vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj;
float4x4 matView;

float aspect;
float invTanHalfFov;
float3 lightDir;

struct VertexInput
{
	float3 position: POSITION0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 position: POSITION0;
	float2 texCoord: TEXCOORD0;
	float3 eyeVec: TEXCOORD1;
	float3 lightDir: TEXCOORD2;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	
	OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);
	OUT.texCoord = IN.texCoord;
	OUT.eyeVec = float3(OUT.position.x * aspect, OUT.position.y, invTanHalfFov);
	OUT.lightDir = -normalize(mul(float4(lightDir, 0.0f), matView).xyz);
	
	return OUT;
}