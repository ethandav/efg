//sky.frag
SamplerState TextureSampler;

TextureCube CubeMap;
Texture2D ColorBuffer;

struct Params 
{
    float4 Position : SV_Position;
    float3 uv : TEXTCOORDS;
};

float4 main(Params input) : SV_Target
{
    float4 FragColor = CubeMap.Sample(TextureSampler, input.uv);
    return FragColor;
}
