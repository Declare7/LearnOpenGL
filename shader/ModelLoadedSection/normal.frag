#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material
{
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    float shininess;
};

struct DirectionLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirectionLight directionLight;
uniform vec3 viewPos;

vec3 calcDirLight(vec3 norm)
{
    vec3 ambient = directionLight.ambient * vec3(texture(material.texture_diffuse0, TexCoords));

    vec3 lightDir = normalize(-directionLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directionLight.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords));

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directionLight.specular * spec * vec3(texture(material.texture_specular0, TexCoords));

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 result = calcDirLight(norm);

    FragColor = vec4(result, 1.0);
}
