// Parallax mapping vertex shader
float4x4 matWorldViewProj	: WorldViewProjection;
float4x4 matWorld			: World;
float4x4 matWorldView;
float3 cameraPos;

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
    float4 hPosition  : POSITION0;
    float2 texCoord   : TEXCOORD0;
    float3 position	  : TEXCOORD1;
    float3 normal	  : TEXCOORD2;
    float3 V		  : TEXCOORD3;
    float3 positionV  : TEXCOORD4;
};
 
  
// VERTEX SHADER
VertexOutput VS_TransformAndTexture(VertexInput IN) 
{
	VertexOutput OUT;
	
    OUT.hPosition    = mul(IN.position, matWorldViewProj);
    OUT.texCoord	 = IN.texCoord;
    OUT.normal		 = mul(float4(IN.normal, 0.0f), matWorld).xyz;
    OUT.position	 = mul(IN.position, matWorld).xyz;
    OUT.V		 	 = cameraPos - OUT.position;
    OUT.positionV	 = mul(IN.position, matWorldView).xyz;
 
    
    return OUT;
}