// Lightning vertex shader
// Copyright (C) Wojciech Toman 2009

float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorld			: World;
float4x4 matWorldView;
float3 cameraPos;

// Input for the VertexShader
struct VertexInput
{ 
    float4 position   : POSITION0;
    float4 diffuse    : COLOR0;
    float2 texCoord   : TEXCOORD0;
    float3 normal     : NORMAL;
};
 
// VertexShader results
struct VertexOutput
{
    float4 position   : POSITION0;
    float4 diffuse	  : COLOR0;
    float2 texCoord   : TEXCOORD0;
    float3 normal	  : TEXCOORD1;
    float3 positionV  : TEXCOORD2;
};
 
  
// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;
	
    OUT.position    = mul(IN.position, matWorldViewProj);
    OUT.texCoord    = IN.texCoord;
    OUT.normal		= mul(float4(IN.normal, 0.0f), matWorld).xyz;
    OUT.diffuse	 	= IN.diffuse;
    OUT.positionV	= mul(IN.position, matWorldView).xyz;
    
    return OUT;
}