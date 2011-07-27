// Morphable terrain vertex shader
// Copyright (C) Wojciech Toman 2010

float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorld			: World;
float4x4 matWorldView;
float3 cameraPos;
float blendAmount = 0.0f;


// Input for the VertexShader
struct VertexInput
{
	float4 position   : POSITION0;
	float2 texCoord   : TEXCOORD0;
	float3 normal     : NORMAL0;
	
	float4 position2  : POSITION1;
	float2 texCoord2  : TEXCOORD1;
	float3 normal2	  : NORMAL1;
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
	
	float4 position = lerp(IN.position, IN.position2, blendAmount);
	float2 texCoord = lerp(IN.texCoord, IN.texCoord2, blendAmount);
	float3 normal	= lerp(IN.normal, IN.normal2, blendAmount);
	
	OUT.position    = mul(position, matWorldViewProj);
	OUT.texCoord    = texCoord;
	OUT.normal		= mul(float4(normal, 0.0f), matWorld).xyz;
	float3 worldPos = mul(position, matWorld).xyz;
	OUT.V		 	= -worldPos + cameraPos;
	OUT.positionV	= mul(position, matWorldView).xyz;
	OUT.altitude 	= worldPos.y;
	
	return OUT;
}