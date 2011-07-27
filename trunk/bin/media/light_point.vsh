// Point-light vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj;
float4x4 matView;

float3 lightPosition = {0.0f, 2.5f, 0.0f};

float aspect;
float invTanHalfFov;


struct VertexInput
{
	float4 position: POSITION0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 position: POSITION0;
	float2 texCoord: TEXCOORD0;
	float3 lightPosition: TEXCOORD1;
	float3 eyeVec: TEXCOORD2;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	
	OUT.position = mul(IN.position, matWorldViewProj);
	OUT.texCoord = IN.texCoord;
	OUT.lightPosition = mul(float4(lightPosition, 1.0f), matView).xyz;
	OUT.eyeVec = float3(OUT.position.x * aspect, OUT.position.y, invTanHalfFov);
	
	return OUT;
}