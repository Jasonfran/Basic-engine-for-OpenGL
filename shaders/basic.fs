#version 330 core

out vec4 colour;

struct Material{
    vec3 ambient;
    sampler2D ambientTexture;
    int usesAmbientTexture;
    vec3 diffuse;
    sampler2D diffuseTexture;
    int usesDiffuseTexture;
    vec3 specular;
    sampler2D specularTexture;
    int usesSpecularTexture;
    sampler2D normalTexture;
    int usesNormalTexture;
    float shininess;
};

struct Pointlight{
    vec3 colour;
    vec3 position;
    float attenuation;
};

in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform Material material;
uniform vec3 cameraPos;
uniform Pointlight light;

uniform sampler2D depthMap;

vec2 poissonDisk[12] = vec2[](
    vec2(0.2630714f, 0.6912614f),
    vec2(-0.1820333f, 0.1705465f),
    vec2(0.9096217f, 0.3265707f),
    vec2(0.3534901f, 0.07025013f),
    vec2(-0.1945064f, 0.9183983f),
    vec2(0.462753f, -0.4803463f),
    vec2(-0.09757187f, -0.4770119f),
    vec2(0.9208652f, -0.1816245f),
    vec2(-0.9852856f, -0.07939659f),
    vec2(-0.6533683f, -0.7022298f),
    vec2(-0.639062f, 0.6613536f),
    vec2(0.1162846f, -0.9582595f)
);

float calcShadow(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal){
    // Need to perspective divide since OpenGL can't do it for us
    vec3 projectedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Depth ranges from 0.0 to 1.0, and tex coords are 0.0 to 1.0. 
    // Putting projectedCoords in that range will be useful
    projectedCoords = projectedCoords * 0.5 + 0.5;
    // Now need to get the closest depth from the light POV
    float closestDepth = texture(depthMap, projectedCoords.xy).r;
    // Now you can get the current fragment depth from the light with the z coordinate
    float currentDepth = projectedCoords.z;
    // If in shadow then return 0.0, if not then return 1.0
   	float bias = min(0.00002 * (1.0 - dot(normal, lightDir)), 0.05);
    float shadow = 0.0;
    for (int i = 0; i < 12; i++){
        if(texture(depthMap, projectedCoords.xy + poissonDisk[i]/800).r < currentDepth - bias){
            shadow +=1.0/12;
        }
    } 
    
    if(projectedCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

float globalAmbience = 0.03;

void main()
{
    vec3 ambient;
    if(material.usesAmbientTexture == 1)
        ambient = vec3(texture(material.ambientTexture, fs_in.TexCoords)) * globalAmbience;
    else
        ambient = material.ambient * globalAmbience;

    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float angle = max(dot(fs_in.Normal, lightDir), 0.0);
    vec3 diffuse;
    if(material.usesDiffuseTexture == 1)
        diffuse = light.colour * angle * vec3(texture(material.diffuseTexture, fs_in.TexCoords));
    else 
        diffuse = light.colour * angle * material.diffuse;

    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(fs_in.Normal, halfwayDir), 0.0), 32);
    vec3 specular;
    if(material.usesSpecularTexture == 1)
        specular = light.colour * spec * vec3(texture(material.specularTexture, fs_in.TexCoords));
    else
       specular = light.colour * spec * material.specular;

    float lightDist = length(light.position - fs_in.FragPos);
    float constant = 1.0f;
    float linear = 0.0014;
    float quadratic = 0.000007;
    float attenuation = 1.0 / (constant + linear * lightDist + quadratic * (lightDist * lightDist));
    float shadow = calcShadow(fs_in.FragPosLightSpace, lightDir, fs_in.Normal);
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));

    vec3 gamma = vec3(1.0/2.2);
    //color = texture(material.ambientTexture, fs_in.TexCoords);
    //colour = vec4(1.0, 0.0, 0.0, 1.0f);
    colour = vec4(pow(result, gamma), 1.0f);
    //if(fs_in.Normal == vec3(0.0, 1.0, 0.0))
        //colour = vec4(0.0, 0.0, 0.0, 1.0);
}
