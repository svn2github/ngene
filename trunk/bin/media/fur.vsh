// Fur vertex shader
// Copyright (C) Wojciech Toman 2008

float furLength = 0.5f;
float UVScale = 1.0f;

float4x4 matWorldViewProj;
float4x4 matWorld;
float4x4 matWorldView;

float3 gravityVec = float3(0.0f, -0.1f, 0.0f); 

float layer = 0.0f; // 0 to 1 for the level


struct VertexInput
{ 
    float4 position   : POSITION0;
    float2 texCoord   : TEXCOORD0;
    float3 normal     : NORMAL;
};

struct VertexOutput
{
    float4 hpos		: POSITION0;
    float2 tex0	    : TEXCOORD0;
    float2 tex1	    : TEXCOORD1;
    float3 normal   : TEXCOORD2;
    float3 positionV: TEXCOORD3;
};

VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT = (VertexOutput)0;

	float3 pos = IN.position.xyz + (IN.normal * furLength);
	float3 normal = normalize(mul(float4(IN.normal, 0.0f), matWorld).xyz);
	
	// Additional gravitation force code
	float3 gravity = mul(float4(gravityVec, 0.0f), matWorld).xyz;
	float k = pow(layer, 3); // We use the pow function, so that only the tips of the hairs bend
	                         // As layer goes from 0 to 1, so by using pow(..) function is still 
	                         // goes form 0 to 1, but it increases faster! exponentially
	pos = pos + gravity * k;
	
	OUT.tex0 = IN.texCoord * UVScale;
	OUT.tex1 = IN.texCoord;
	
	OUT.hpos = mul(float4(pos, 1.0f), matWorldViewProj);
    OUT.normal = normal;
    OUT.positionV = mul(float4(pos, 1.0f), matWorldView).xyz;
    
    return OUT;
}