// Water vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorld			: World;
float4x4 matWorldView;
float3 cameraPos;

float waterLevel = 0.0f;

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
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
};
 
  
// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT = (VertexOutput)0;
	OUT.position = mul(IN.position, matWorldViewProj);
	OUT.texCoord = IN.texCoord;
    
    return OUT;
}