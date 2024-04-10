float3 lightColor;
float lightIntensity;

SamplerState TextureSampler;

Texture2D AlbedoBuffer;
Texture2D ColorBuffer;

struct Params 
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 FragPos : POSITION;
    float3 Color : COLOR;
    float2 uv : TEXTCOORDS;
};

float4 main(Params input) : SV_Target
{
    float3 texColor = AlbedoBuffer.Sample(TextureSampler, input.uv).xyz;
    
    // Ambient Lighting
    float ambientStrength = lightIntensity;
    float3 ambient = ambientStrength * lightColor;

    // Final color calculation
    float3 result = ambient * texColor;
    float4 FragColor = float4(result, 1.0);

    return FragColor;
}

float4 resolve(in float4 pos : SV_Position) : SV_Target
{
    return ColorBuffer.Load(int3(pos.xy, 0));
}
