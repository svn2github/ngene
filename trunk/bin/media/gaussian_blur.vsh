// Gaussian blur vertex shader
// Copyright (C) Wojciech Toman 2008

sampler aoTex: register(s1);

float2 pixel_size = {1.0f / 800.0f, 1.0f / 600.0f};
float2 direction = {0.0f, 1.0f};
float blurStrength = 1.5f;

float4x4 matWorldViewProj;


struct VertexInput
{
	float3 position: POSITION; 
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
    float4 position   : POSITION;
    float2 texCoord[7]: TEXCOORD0;
};

VertexOutput VS_GaussianBlur(VertexInput IN)
{
    VertexOutput OUT = (VertexOutput)0;
    OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);
    
	float2 texelSize = blurStrength * pixel_size;
    float2 s = IN.texCoord;
    for(int i = 0; i < 3; i++)
    {
    	OUT.texCoord[i] = s - texelSize * i * direction;
    	OUT.texCoord[i + 4] = s + texelSize * i * direction;
    }
    OUT.texCoord[3] = s;
    
    
    return OUT;
}