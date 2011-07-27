// Non-lightable and not textured material vertex shader
// Copyright (C) Wojciech Toman 2009

float4x4 matWorldViewProj;

// Input for the VertexShader
struct VertexInput
{ 
    float3 position   : POSITION0;
    float4 diffuse    : COLOR0;
};
 
// VertexShader results
struct VertexOutput
{
    float4 position   : POSITION0;
    float4 colour	  : COLOR0;
};
 
  
// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;
	
    OUT.position    = mul(float4(IN.position, 1.0f), matWorldViewProj);
    OUT.colour		= IN.diffuse;

    return OUT;
}