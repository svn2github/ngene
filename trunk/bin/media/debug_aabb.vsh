// Debug aabb vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj;
float4x4 matWorld;
float4 colour = {1.0f, 0.5f, 1.0f, 0.5f};

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
    float4 colour	  : COLOR0;
};
 

// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;
	
    float3 normal = normalize(mul(float4(IN.normal, 0.0f), matWorld)).xyz;
    float3 lightDir = normalize(float3(1.0f, 0.5f, 0.25f));
    float3 diffuse = min(saturate(dot(normal, lightDir)) + 0.3f, 1.0f) * colour.xyz;
	
    OUT.position    = mul(IN.position, matWorldViewProj);
    OUT.colour		= float4(diffuse, colour.w);


    return OUT;
}