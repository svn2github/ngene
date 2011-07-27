// Downsample vertex shader
// Copyright (C) Wojciech Toman 2008

float2 pixel_size = {1.0f / 800.0f, 1.0f / 600.0f};
float scale = 0.5f;

float4x4 matWorldViewProj;


struct VertexInput
{
	float3 position: POSITION0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 position: POSITION0;
	float2 texCoord[4]: TEXCOORD0;
};


VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	
	float2 texelSize = scale * pixel_size;
	float2 tex = (float2)IN.texCoord;
    
    OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);;
    OUT.texCoord[0] = tex;
    OUT.texCoord[1] = tex + float2(2, 0) * texelSize;
    OUT.texCoord[2] = tex + float2(2, 2) * texelSize;
    OUT.texCoord[3] = tex + float2(0, 2) * texelSize;
    
    return OUT;
}