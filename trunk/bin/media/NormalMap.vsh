float4x4 WorldViewProj	: WorldViewProjection;
float4x4 WorldViewIT	: WorldViewInverseTranspose;
float4x4 World			: World;
float4x4 ViewInverse	: ViewInverse;

// Input for the VertexShader
struct VertexInput
{ 
    float4 position   : POSITION0;
    float2 texCoord   : TEXCOORD0;
    float3 normal     : NORMAL;
    float3 tangent    : TANGENT;
    float3 binormal	  : BINORMAL;
};
 
// VertexShader results
struct VertexOutput
{
    float4 position	   : POSITION0;
    float2 texCoord    : TEXCOORD0;
    float3 eyeVec	   : TEXCOORD1;
    float3 lightVec    : TEXCOORD2;
    float3 normal 	   : TEXCOORD3;
    float3 tangent	   : TEXCOORD4;
    float3 binormal    : TEXCOORD5;
};
 
  
// VERTEX SHADER
VertexOutput VS_TransformAndTexture(VertexInput IN) 
{
	VertexOutput OUT;
	
    OUT.position    = mul(IN.position, WorldViewProj);
    float4 posWorld = mul(IN.position, World);
 
    // Calculate the TBN
    OUT.normal  = mul(IN.normal, WorldViewIT);
    OUT.tangent = mul(IN.tangent, WorldViewIT);
    OUT.binormal= mul(IN.binormal, WorldViewIT);    
    OUT.eyeVec	 = ViewInverse[3].xyz - posWorld;
    OUT.lightVec = normalize(lightDir);
    OUT.texCoord = IN.texCoord;
    
    
    return OUT;
}