float3 viewPos;

SamplerState TextureSampler;

Texture2D AlbedoBuffer;
Texture2D ColorBuffer;

bool useTexture;

cbuffer MaterialBuffer : register(b0)
{
    struct Material
    {
        float4  ambient;
        float4  diffuse;
        float4  specular;
        float shininess;
    };
    Material material;
};

cbuffer LightBuffer : register(b1)
{
    struct Light
    {
        float4 position;
        float4  specular;
        float4  ambientColor;
        float4  diffuseColor;
    };
    Light light;
}

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

    // Ambient Lighting
    float3 ambient = light.ambientColor.xyz * material.ambient.xyz;

    // Diffuse lighting
    float3 diffuse;
    float3 FragPos = input.FragPos;
    float3 normal = normalize(input.Normal);
    float3 lightDir = normalize(light.position.xyz - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    if(useTexture)
    {
        diffuse = AlbedoBuffer.Sample(TextureSampler, input.uv).xyz;
    }
    else
    {
        diffuse = light.diffuseColor.xyz * (diff * material.diffuse.xyz);
    }

    // Specular Lighting
    float3 viewDir = normalize(viewPos - FragPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float3 specular = light.specular.xyz * (spec * material.specular.xyz);

    // Final color calculation
    float3 result = (ambient + diffuse + specular);
    float4 FragColor = float4(result, 1.0);

    return FragColor;

}

float4 resolve(in float4 pos : SV_Position) : SV_Target
{
    return ColorBuffer.Load(int3(pos.xy, 0));
}
