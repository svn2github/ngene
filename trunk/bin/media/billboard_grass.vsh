// Billboard grass vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldView;
float4x4 matWorld;
float4x4 matView;
float4x4 matProj;
float3 camPosition = {0.0f, 0.0f, 0.0f};
float timer = 0.0f;

float wind = 0.1f;

float4 right = {1.0f, 0.0f, 0.0f, 0.0f};

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
	float4 diffuse  : COLOR0;
	float2 texCoord : TEXCOORD0;
	float3 normal   : TEXCOORD1;
	float3 positionV: TEXCOORD2;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;

	float4 pos = mul(IN.position, matWorldView);
	float4 up = mul(float4(0.0f, 1.0f, 0.0f, 0.0f), matView);
	up.w = 0;
	up = normalize(up);
	
	pos += IN.texCoord.x * right + IN.texCoord.y * up;
	if(IN.normal.y > 0.0f)
		pos.x += sin(timer / (3000.0f + IN.position.y) + IN.position.x * IN.position.y) * wind;

	
	OUT.texCoord = 1.0f - (IN.texCoord / abs(IN.texCoord) + 1.0f) * 0.5f;
	OUT.diffuse = IN.diffuse;
	OUT.normal = mul(float4(IN.normal.x, IN.normal.y, IN.normal.z, 0.0f), matWorld).xyz;
	OUT.positionV = pos.xyz;
	OUT.hPosition = mul(float4(pos.xyz, 1.0f), matProj);

	return OUT;
}