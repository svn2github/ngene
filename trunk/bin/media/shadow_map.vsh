// Shadow map preprocessing vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj: WorldViewProjection;

// Input for the VertexShader
struct VertexInput
{
	float4 position   : POSITION0;
};

// VertexShader results
struct VertexOutput
{
	float4 position   : POSITION0;
	float4 positionV  : TEXCOORD0;
};


// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;
	
	OUT.position  = mul(IN.position, matWorldViewProj);
	OUT.positionV = mul(IN.position, matWorldViewProj);
	
	return OUT;
}