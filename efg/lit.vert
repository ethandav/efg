//cube.vert
float4x4 g_ViewProjection;
float4x4 transform;
float3x3 normalMatrix;

struct Params
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 FragPos : POSITION;
    float3 Color : COLOR;
    float2 uv : TEXTCOORDS;
};

Params main(float3 pos : Position, float3 normal: NORMAL, float2 uv : TEXCOORDS)
{
    // Transform the position to world space
    float4 worldPos = mul(transform, float4(pos, 1.0f));
    float4 clipPos = mul(g_ViewProjection, worldPos);

    // Create the output structure
    Params output;
    output.Position = clipPos;
    //output.Normal = normalize(mul(normalMatrix, normal));
    output.Normal = normal;
    output.uv = uv;
    output.FragPos = worldPos.xyz;
    output.Color = float3(1.0f, 1.0f, 1.0f);

    return output;
}

float4 resolve(in uint idx : SV_VertexID) : SV_Position
{
    return 1.0f - float4(4.0f * (idx & 1), 4.0f * (idx >> 1), 1.0f, 0.0f);
}
