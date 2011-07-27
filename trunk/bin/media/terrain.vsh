// Terrain vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorld			: World;
float4x4 matWorldView;
float3 cameraPos;


// Input for the VertexShader
struct VertexInput
{
	float4 position   : POSITION0;
	float2 texCoord   : TEXCOORD0;
	float3 normal     : NORMAL;
};

// VertexShader results
struct VertexOutput
{
	float4 position   : POSITION0;
	float2 texCoord   : TEXCOORD0;
	float3 normal	  : TEXCOORD1;
	float3 V		  : TEXCOORD2;
	float3 positionV  : TEXCOORD3;
	float altitude	  : TEXCOORD4;
};


// VERTEX SHADER
VertexOutput main(VertexInput IN)
{
	VertexOutput OUT;
	
	OUT.position    = mul(IN.position, matWorldViewProj);
	OUT.texCoord    = IN.texCoord;
	OUT.normal		= mul(float4(IN.normal, 0.0f), matWorld).xyz;
	float3 worldPos = mul(IN.position, matWorld).xyz;
	OUT.V		 	= -worldPos + cameraPos;
	OUT.positionV	= mul(IN.position, matWorldView).xyz;
	OUT.altitude 	= worldPos.y;
	
	return OUT;
}