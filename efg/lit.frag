float3 viewPos;

SamplerState TextureSampler;

Texture2D ColorBuffer;
Texture2D diffuseMap;
Texture2D specularMap;

bool useDiffuseMap;
bool useSpecMap;

struct Directional
{
    float3 direction;
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

StructuredBuffer<Directional> dirLights;

struct PointLight
{
    float3 position;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float constant;
    float pLinear;
    float quadratic;
};
uint32_t numPointLights;
StructuredBuffer<PointLight> pointLights;

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

struct Object
{
    float3  ambient;
    float3  diffuse;
    float3  specular;
    float shininess;
};

struct Params 
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 FragPos : POSITION;
    float3 Color : COLOR;
    float2 uv : TEXTCOORDS;
};

float3 CalculateDirectionalLight(float3 normal, float3 viewDir, Object object)
{
    // Ambient Lighting
    float3 ambient = dirLights[0].ambient.xyz * object.ambient;

    // Diffuse lighting
    float3 lightDir = normalize(-dirLights[0].direction);
    float diff = max(dot(normal, lightDir), 0.0);
    float3 diffuse = dirLights[0].diffuse.xyz * (diff * object.diffuse.xyz);

    // Specular Lighting
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float3 specular = dirLights[0].specular.xyz * (spec * object.specular.xyz);

    return (ambient + diffuse + specular);
}

float3 calculatePointLight(PointLight light, float3 normal, float3 fragPos, float3 viewDir, Object object)
{
    float3 lightDir = normalize(light.position - fragPos);

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.pLinear * distance + light.quadratic * (distance * distance));

    float3 ambient = light.ambient * object.ambient;
    float3 diffuse = light.diffuse * (diff * object.diffuse.xyz);
    float3 specular = light.specular * (spec * object.specular.xyz);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

float4 main(Params input) : SV_Target
{
    Object object;
    if(useDiffuseMap)
    {
        object.diffuse = diffuseMap.Sample(TextureSampler, input.uv).xyz;
        object.ambient = object.diffuse;
    }
    else
    {
        object.diffuse = material.diffuse.xyz;
        object.ambient = material.ambient.xyz;
    }

    if(useSpecMap)
    {
        object.specular = specularMap.Sample(TextureSampler, input.uv).xyz;
    }
    else
    {
        object.specular = material.specular.xyz;
    }

    // Properties
    float3 normal = normalize(input.Normal);
    float3 viewDir = normalize(viewPos - input.FragPos);

    float3 result;
    result = CalculateDirectionalLight(normal, viewDir, object);

    for(int i = 0; i < numPointLights; ++i)
    {
        result += calculatePointLight(pointLights[i], normal, input.FragPos, viewDir, object);
    }

    float4 FragColor = float4(result, 1.0f);

    return FragColor;
}

float4 resolve(in float4 pos : SV_Position) : SV_Target
{
    return ColorBuffer.Load(int3(pos.xy, 0));
}
