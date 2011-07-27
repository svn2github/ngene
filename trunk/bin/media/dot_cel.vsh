// Cel shading vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matViewProj;
float4x4 matWorld;
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
    float3 positionV  : TEXCOORD2;
    float scale		  : TEXCOORD3;
	float3 V		  : TEXCOORD4;
};
 
  
// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;
	
    float4 worldPos = mul(IN.position, matWorld);
   	float3 eyeDir 	= cameraPos - worldPos.xyz;
   	
    OUT.position    = mul(worldPos, matViewProj);
    OUT.texCoord    = IN.texCoord;
    OUT.V			= eyeDir;
    OUT.normal		= mul(float4(IN.normal, 0.0f), matWorld).xyz;
    OUT.positionV	= mul(IN.position, matWorldView).xyz;
    OUT.scale		= dot(normalize(eyeDir), normalize(OUT.normal));
    
    return OUT;
}