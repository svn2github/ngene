// Light-shaft billboard vertex shader
// Copyright (C) Wojciech Toman 2009

float4x4 matWorldViewProj;
float4x4 matWorldView;
float4x4 matProj;
float3 camPosition = {0.0f, 0.0f, 0.0f};
float timer = 0.0f;

float fractionOfMaxShells = 1.0f;

float3 minBounds;
float3 maxBounds;

float4x4 matViewWorldLight;
float4x4 matViewWorldLightProj;
float4x4 matViewWorldLightProjBias;
float4x4 matViewWorldLightProjBiasAdj;
float4x4 matViewWorldLightProjScroll1;
float4x4 matViewWorldLightProjScroll2;


struct VertexInput
{
	float4 position: POSITION0;
	float3 normal:   NORMAL;
	float4 diffuse:  COLOR0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 hPosition:POSITION0;
	float4 diffuse : COLOR0;
	float4 texCoordProj: TEXCOORD0;
	float4 texCoordProjAdj: TEXCOORD1;
	float4 positionV: TEXCOORD2;
	float4 texCoordProjScroll1: TEXCOORD3;
	float4 texCoordProjScroll2: TEXCOORD4;
};


VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	
	float4 stretchedMaxBounds = float4(maxBounds, 0.0f);
	stretchedMaxBounds.z = stretchedMaxBounds.z / fractionOfMaxShells;
	
	float4 pos = float4(minBounds, 0.0f) * IN.position + (stretchedMaxBounds * (1.0f - IN.position));
	pos.w = 1.0f;
	
	OUT.hPosition = mul(pos, matProj);
	OUT.diffuse = IN.diffuse;

	OUT.positionV.xyz = mul(pos, matViewWorldLight).xyz;
	OUT.texCoordProj = mul(pos, matViewWorldLightProjBiasAdj);
	OUT.texCoordProjAdj = mul(pos, matViewWorldLightProjBiasAdj);
	OUT.positionV.w = OUT.texCoordProjAdj.z / OUT.texCoordProjAdj.w;
	OUT.texCoordProjScroll1 = mul(pos, matViewWorldLightProjScroll1);
	OUT.texCoordProjScroll2 = mul(pos, matViewWorldLightProjScroll2);

	return OUT;
}