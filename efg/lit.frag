float3 viewPos;

SamplerState TextureSampler;

Texture2D ColorBuffer;
Texture2D diffuseMap;
Texture2D specularMap;

bool useDiffuseMap;
bool useSpecMap;

struct dirLight
{
    float3 direction;
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

StructuredBuffer<dirLight> dirLights;

cbuffer MaterialBuffer : register(b0)
{
    struct MaterialProperties
    {
        float4  ambient;
        float4  diffuse;
        float4  specular;
        float shininess;
    };
    MaterialProperties material;
};

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
    float3 objectAmbient, objectDiffuse, objectSpecular;
    if(useDiffuseMap)
    {
        objectDiffuse = diffuseMap.Sample(TextureSampler, input.uv).xyz;
        objectAmbient = objectDiffuse;
    }
    else
    {
        objectDiffuse = material.diffuse.xyz;
        objectAmbient = material.ambient.xyz;
    }

    if(useSpecMap)
    {
        objectSpecular = specularMap.Sample(TextureSampler, input.uv).xyz;
    }
    else
    {
        objectSpecular = material.specular.xyz;
    }

    // Ambient Lighting
    //float3 ambient = light.ambientColor.xyz * objectAmbient;
    float3 ambient = dirLights[0].ambient.xyz * objectAmbient;

    // Diffuse lighting
    float3 diffuse;
    float3 FragPos = input.FragPos;
    float3 normal = normalize(input.Normal);
    //float3 lightDir = normalize(light.position.xyz - FragPos);
    float3 lightDir = normalize(-dirLights[0].direction);
    float diff = max(dot(normal, lightDir), 0.0);
    //diffuse = light.diffuseColor.xyz * (diff * objectDiffuse.xyz);
    diffuse = dirLights[0].diffuse.xyz * (diff * objectDiffuse.xyz);

    // Specular Lighting
    float3 viewDir = normalize(viewPos - FragPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //float3 specular = light.specular.xyz * (spec * objectSpecular.xyz);
    float3 specular = dirLights[0].specular.xyz * (spec * objectSpecular.xyz);

    // Final color calculation
    float3 result = (ambient + diffuse + specular);
    float4 FragColor = float4(result, 1.0);

    return FragColor;
}

float4 resolve(in float4 pos : SV_Position) : SV_Target
{
    return ColorBuffer.Load(int3(pos.xy, 0));
}
