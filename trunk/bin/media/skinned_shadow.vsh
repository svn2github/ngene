// Skinning vertex shader
// Copyright (C) Wojciech Toman 2009

float4x4 matWorldViewProj;

/// Number of bones affecting single vertex
static const int bonesNum = 4;

/// Bones matrices
static const int MAX_MATRICES = 40;
float4x3 boneMatrices[MAX_MATRICES];

// Input for the VertexShader
struct VertexInput
{ 
    float4 position      : POSITION0;
    float2 texCoord      : TEXCOORD0;
    float4 diffuse       : COLOR0;
    float3 normal        : NORMAL;
    float4 bonesWeights  : BLENDWEIGHT;
    float4 bonesIndices  : BLENDINDICES;
};
 
// VertexShader results
struct VertexOutput
{
    float4 position   : POSITION0;
    float4 positionV  : TEXCOORD0;
};
 
  
// VERTEX SHADER
VertexOutput main(VertexInput IN) 
{
	float lastWeight = 0.0f;
	int4 indices = D3DCOLORtoUBYTE4(IN.bonesIndices);

    // cast the vectors to arrays for use in the for loop below
    float bonesWeightsArray[4] = (float[4])IN.bonesWeights;
    int bonesIndicesArray[4]   = (int[4])indices;
    
    float3 pos = 0.0f;
    
    // calculate position and normal vectors
    for(int i = 0; i < bonesNum - 1; i++)
    {
        lastWeight = lastWeight + bonesWeightsArray[i];
        
        pos += mul(IN.position, boneMatrices[bonesIndicesArray[i]]).xyz * bonesWeightsArray[i];
    }
    
    lastWeight = 1.0f - lastWeight;

    pos += (mul(IN.position, boneMatrices[bonesIndicesArray[bonesNum - 1]]).xyz * lastWeight);
    
    
    VertexOutput OUT;
    
    OUT.position = mul(float4(pos, 1.0f), matWorldViewProj);
    OUT.positionV= mul(float4(pos, 1.0f), matWorldViewProj);
    
    
    return OUT;
}