// Cloud vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorld;
float4x4 matWorldView;

float timer = 0.0f;
float2 wind = {1.0f, 0.5f};

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
	float4 hPosition  : POSITION0;
    float2 texCoord   : TEXCOORD0;
    float3 position	  : TEXCOORD1;
    float3 positionV  : TEXCOORD2;
    float2 texCoord2  : TEXCOORD3;
};
 
  
// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;
	
    OUT.hPosition    = mul(IN.position, matWorldViewProj);
    OUT.texCoord	 = IN.texCoord + timer / 1000000.0f * wind;
    OUT.texCoord2	 = IN.texCoord;
    OUT.position	 = mul(IN.position, matWorld).xyz;
    OUT.positionV	 = mul(IN.position, matWorldView).xyz;
 
    
    return OUT;
}