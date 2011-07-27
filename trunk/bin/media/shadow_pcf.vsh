// Shadow map preprocessing vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorldView;

float aspect;
float invTanHalfFov;

// Input for the VertexShader
struct VertexInput
{ 
    float4 position   : POSITION0;
    float2 texCoord   : TEXCOORD0;
};
 
// VertexShader results
struct VertexOutput
{
    float4 position   : POSITION0;
    float2 texCoord   : TEXCOORD0;
    float3 eyeVec	  : TEXCOORD1;
};
 
  
// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;
	
    OUT.position    = mul(IN.position, matWorldViewProj);
    OUT.texCoord	= IN.texCoord;
    OUT.eyeVec 		= float3(OUT.position.x * aspect, OUT.position.y, invTanHalfFov);
    
    return OUT;
}