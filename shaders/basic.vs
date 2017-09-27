#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position =  projection * view * model * vec4(Position, 1.0); 
    vs_out.FragPos = vec3(model * vec4(Position, 1.0));
    vs_out.Normal = normalize(transpose(inverse(mat3(model))) * Normal); // I believe this makes sure the normals are right in non-uniform scaling
                                                              // I think some people use the model*view matrix but this will light in world space, not view space
    vs_out.TexCoords = TexCoords;

   vec3 T = normalize(vec3(model * vec4(Tangent,   0.0)));
   vec3 B = normalize(vec3(model * vec4(Bitangent, 0.0)));
   vec3 N = normalize(vec3(model * vec4(vs_out.Normal,    0.0)));
   vs_out.TBN = mat3(T, B, N);
}
