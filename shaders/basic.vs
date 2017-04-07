#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position =  projection * view * model * vec4(Position, 1.0); 
    vs_out.FragPos = vec3(model * vec4(Position, 1.0));
    vs_out.Normal = normalize(transpose(inverse(mat3(model))) * Normal); // I believe this makes sure the normals are right in non-uniform scaling
                                                              // Some people use the model*view matrix but I will light in world space, not view space
    vs_out.TexCoords = TexCoords;
}
