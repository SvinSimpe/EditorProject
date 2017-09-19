
struct PixelInput
{
    float4 position : SV_POSITION;
    // Add normal and texcoord
};

float4 main( PixelInput pixelInput ) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}