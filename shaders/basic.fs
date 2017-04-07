#version 330 core

out vec4 colour;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Pointlight{
    vec3 colour;
    vec3 position;
    float attenuation;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform Material material;
uniform vec3 cameraPos;
uniform Pointlight light;
void main()
{
    vec3 ambient = light.colour * material.ambient;

    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float angle = max(dot(fs_in.Normal, lightDir), 0.0);
    vec3 diffuse = light.colour * (angle * material.diffuse);

    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, fs_in.Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.colour * (spec * material.specular);

    float lightDist = length(light.position - fs_in.FragPos);
    float constant = 1.0f;
    float linear = 0.22;
    float quadratic = 0.20;
    float attenuation = 1.0 / (constant + linear * lightDist + quadratic * (lightDist * lightDist));

    vec3 result = attenuation * (ambient + diffuse + specular);

    vec3 gamma = vec3(1.0/2.2);
    //color = texture(material.diffuse1, fs_in.TexCoords);
    //colour = vec4(result, 1.0f);
    colour = vec4(pow(result, gamma), 1.0f);
}
