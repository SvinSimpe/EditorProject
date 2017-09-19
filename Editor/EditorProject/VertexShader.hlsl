
cbuffer PerFrame : register(b0)
{
    column_major float4x4 view;
    column_major float4x4 projection;
};

cbuffer PerObject : register(b1)
{
    column_major float4x4 world;
};

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    // Add normal and texcoord
};


PixelInput main( VertexInput vertexInput )
{
    PixelInput pixelInput = (PixelInput)0;

    pixelInput.position = mul( float4(vertexInput.position, 1.0f), world );
    pixelInput.position = mul( pixelInput.position, view );
    pixelInput.position = mul( pixelInput.position, projection );

    return pixelInput;
}