// Sky vertex shader
// Copyright (C) Wojciech Toman 2008

float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorld			: World;
float4x4 matWorldView;
float3 cameraPos;


struct VertexInput
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal   : NORMAL;
};
                                                                                                               
struct VertexOutput
{                                                                                                           
	float4 hPosition  : POSITION0;
    float2 texCoord   : TEXCOORD0;
    float2 texCoord2  : TEXCOORD1;
    float3 direction  : TEXCOORD2;
    float3 positionV  : TEXCOORD3;
};

                                                                                                               
VertexOutput main(VertexInput IN)
{
	VertexOutput OUT;
	
	float3 direction = IN.position.xyz;
	if(direction.y < 0.0f)
		direction.y = 0.0f;
		
	OUT.direction 	 = direction;                                                                                                               
	OUT.hPosition    = mul(IN.position, matWorldViewProj);	
    OUT.texCoord	 = IN.texCoord;
    OUT.texCoord2	 = IN.texCoord + float2(0.25f, 0);
    OUT.positionV	 = mul(IN.position, matWorldView).xyz;
                                                                                                               
	return OUT;                                                                                             
}