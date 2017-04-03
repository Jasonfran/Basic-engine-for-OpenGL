#version 330 core

out vec4 color;

struct Material{
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D specular1;
};

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform Material material;

void main()
{
    color = texture(material.diffuse1, fs_in.TexCoords);
}
