// Vegetation animation vertex shader
// Copyright (C) Wojciech Toman 2007

float4x4 matViewProj;
float4x4 matWorld;
float4x4 matView;
float3 cameraPos;
float timer;


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
	float3 V		  : TEXCOORD2;
	float3 positionV  : TEXCOORD3;
};


float bias = 0.0f;
float wind = 0.006f;
float chaos = 0.1f;

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
	OUT.texCoord    = IN.texCoord;
	OUT.normal		= mul(float4(IN.normal, 0.0f), matWorld).xyz;
	float3 worldPos = mul(pos, matWorld).xyz;
	OUT.V		 	= -worldPos + cameraPos;
	OUT.positionV	= mul(pos, matView).xyz;
	
	return OUT;
}