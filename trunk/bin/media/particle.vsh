// Soft-Particle vertex shader
// Copyright (C) Wojciech Toman 2007

float4x4 matViewProj;
float4x4 matViewInv;
float4x4 matView;


struct VertexInput
{
	float4 position: POSITION0;
	float3 normal:   NORMAL0;
	float4 diffuse:  COLOR0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 hPosition:POSITION0;
	float4 diffuse:  COLOR0;
	float4 texCoord: TEXCOORD0;
	float4 positionV:TEXCOORD1;
	float4 position: TEXCOORD2;
	float  radius:   TEXCOORD3;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;

	float radius = IN.normal.z * 0.5f;
	float4 pos = IN.position;
	float angle = IN.texCoord.x;
	
	
	float3 position = float3(IN.normal.xy, 0.0) * radius;
	position = (mul(position, (float3x3)matViewInv) + pos.xyz).xyz;
	

	OUT.hPosition = mul(float4(position, 1.0), matViewProj);
	OUT.position = OUT.hPosition;
	OUT.positionV = mul(float4(position, 1.0), matView).xyzw;

	OUT.texCoord = float4(0.5 * IN.normal.xy + 0.5, 0.0, 0.0);
	
	float2 origin = float2(0.5, 0.5f);
	float2 temp;
	pos.xy = OUT.texCoord.xy;
	temp.x = origin.x + (pos.x - origin.x) * cos(angle) - (pos.y - origin.y) * sin(angle);
	temp.y = origin.y + (pos.x - origin.x) * sin(angle) + (pos.y - origin.y) * cos(angle);
	
	OUT.texCoord.xy = temp.xy;
	OUT.radius = radius;
	OUT.diffuse = IN.diffuse;

	return OUT;
}