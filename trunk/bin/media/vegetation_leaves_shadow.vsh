// Vegetation animation shadow vertex shader
// Copyright (C) Wojciech Toman 2009

float4x4 matWorld;
float4x4 matViewProj;

float timer;
float bias = 0.0f;
float wind = 0.006f;
float chaos = 0.1f;


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
    float4 positionV  : TEXCOORD0;
};


// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	VertexOutput OUT;

	float4 pos = mul(IN.position, matWorld);
	float amplitude = wind * (pos.y - bias - matWorld[3][1]);
	if(amplitude < 0.0f)
		amplitude = 0.0f;
	float time = timer * 0.001f;
	float4 wave = float4(sin(time + pos.x * chaos),
						 0.0f,
						 cos(time + pos.z * chaos),
						 0.0f);
	pos += wave * amplitude;
	
    OUT.position    = mul(pos, matViewProj);
    OUT.positionV	= mul(pos, matViewProj);
    
    return OUT;
}