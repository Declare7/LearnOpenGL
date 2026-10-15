#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOffCos;
    float outerCutOffCos;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    //环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 lightDir = normalize(light.position - FragPos);

    float dist = length(light.position - FragPos);
    float attenuation = 1.0f/ (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    float thetaCos = dot(lightDir, normalize(light.direction));
    float epsilonCos = light.cutOffCos - light.outerCutOffCos;
    float intensity = clamp((thetaCos - light.outerCutOffCos)/ epsilonCos, 0.0f, 1.0f);

    //漫反射
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

    //镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    vec3 result = ambient * attenuation + (diffuse + specular) * attenuation * intensity;
    FragColor = vec4(result, 1.0);
}
