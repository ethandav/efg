//sky.vert
float4x4 g_View;
float4x4 g_Projection;
float4x4 transform;

struct Params
{
    float4 Position : SV_Position;
    float3 uv : TEXTCOORDS;
};

Params main(float3 pos : Position)
{
    // Transform the position to world space
    float4x4 viewProjection = mul(g_Projection, g_View);
    float4 worldPos = mul(transform, float4(pos, 1.0f));
    float4 clipPos = mul(viewProjection, worldPos);

    // Create the output structure
    Params output;
    output.Position = clipPos;
    output.uv = pos;

    return output;
}
