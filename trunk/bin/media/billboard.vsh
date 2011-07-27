// Billboard vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj;
float4x4 matWorldView;
float4x4 matProj;
float3 camPosition = {0.0f, 0.0f, 0.0f};
float timer = 0.0f;

float3 right = {1.0f, 0.0f, 0.0f, 0.0f};
float3 up = {0.0f, 1.0f, 0.0f, 0.0f};

struct VertexInput
{
	float4 position: POSITION0;
	float3 normal:   NORMAL;
	float4 diffuse:  COLOR0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 hPosition:POSITION0;
	float4 diffuse : COLOR0;
	float2 texCoord: TEXCOORD0;
	float3 normal  : TEXCOORD1;
	float3 positionV: TEXCOORD2;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;

	float4 pos = IN.position;
 
	pos = mul(pos, matWorldView);

	float3 P = pos.xyz;
	float3 at = normalize(P);
	
	float3 Q = P;
	Q += IN.texCoord.x * right + IN.texCoord.y * up;

	pos = mul(float4(Q, 1.0f), matProj);

	OUT.hPosition = pos;
	OUT.texCoord = 1.0f - (IN.texCoord / abs(IN.texCoord) + 1.0f) * 0.5f;
	OUT.diffuse = IN.diffuse;
	OUT.normal = -at;
	OUT.positionV = Q.xyz;

	return OUT;
}