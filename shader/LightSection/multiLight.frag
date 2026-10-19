#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOffCos;
    float outerCutOffCos;
};


#define POINT_LIGHT_NUM 4

uniform Material material;
uniform DirectionLight dirLight;
uniform PointLight pointLights[POINT_LIGHT_NUM];
uniform SpotLight spotLight;
uniform vec3 viewPos;

vec3 calcDirLight(vec3 norm)
{
    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specular, TexCoord));

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 norm, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    float dist = length(light.position - FragPos);
    float attenuation = 1.0f/ (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    vec3 result = (ambient + diffuse + specular) * attenuation;

    return result;
}

vec3 calcSpotLight(vec3 norm, vec3 viewDir)
{
    //环境光
    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 lightDir = normalize(spotLight.position - FragPos);

    float dist = length(spotLight.position - FragPos);
    float attenuation = 1.0f/ (spotLight.constant + spotLight.linear * dist + spotLight.quadratic * (dist * dist));

    float thetaCos = dot(lightDir, normalize(spotLight.direction));
    float epsilonCos = spotLight.cutOffCos - spotLight.outerCutOffCos;
    float intensity = clamp((thetaCos - spotLight.outerCutOffCos)/ epsilonCos, 0.0f, 1.0f);

    //漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    //镜面反射
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, TexCoord));

    return ambient * attenuation + (diffuse + specular) * attenuation * intensity;
}


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirLight(norm);

    for(int i= 0; i< POINT_LIGHT_NUM; ++i)
    {
        result += calcPointLight(pointLights[i], norm, viewDir);
    }

    result += calcSpotLight(norm, viewDir);

    FragColor = vec4(result, 1.0);
}
