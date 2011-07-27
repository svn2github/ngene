float4x4 matWorldViewProj;

struct VertexInput
{
	float3 position: POSITION0;
	float3 normal:   NORMAL;
	float4 diffuse:  COLOR0;
	float2 texCoord: TEXCOORD0;
};

struct VertexOutput
{
	float4 position: POSITION0;
	float4 diffuse:  COLOR0;
	float2 texCoord: TEXCOORD0;
	float3 normal: TEXCOORD1;
};

VertexOutput main(VertexInput IN)
{
	VertexOutput OUT = (VertexOutput)0;
	OUT.position = mul(float4(IN.position, 1.0f), matWorldViewProj);
	OUT.diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	OUT.texCoord = IN.texCoord;

	return OUT;
}