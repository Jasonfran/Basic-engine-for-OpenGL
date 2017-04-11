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
    vec4 FragPosLightSpace;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform Material material;
uniform vec3 cameraPos;
uniform Pointlight light;

uniform sampler2D depthMap;

float calcShadow(vec4 fragPosLightSpace){
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
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  

    if(projectedCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

float globalAmbience = 0.03;

void main()
{
    vec3 ambient = material.ambient * globalAmbience;

    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float angle = max(dot(fs_in.Normal, lightDir), 0.0);
    vec3 diffuse = light.colour * (angle * material.diffuse);

    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(fs_in.Normal, halfwayDir), 0.0), 32);
    vec3 specular = light.colour * (spec * material.specular);

    float lightDist = length(light.position - fs_in.FragPos);
    float constant = 1.0f;
    float linear = 0.09;
    float quadratic = 0.032;
    float attenuation = 1.0 / (constant + linear * lightDist + quadratic * (lightDist * lightDist));
    float shadow = calcShadow(fs_in.FragPosLightSpace);
    vec3 result = attenuation * (ambient + (1.0 - shadow) * (diffuse + specular));

    vec3 gamma = vec3(1.0/2.2);
    //color = texture(material.diffuse1, fs_in.TexCoords);
    //colour = vec4(result, 1.0f);
    colour = vec4(pow(result, gamma), 1.0f);
    //if(fs_in.Normal == vec3(0.0, 1.0, 0.0))
        //colour = vec4(0.0, 0.0, 0.0, 1.0);
}
