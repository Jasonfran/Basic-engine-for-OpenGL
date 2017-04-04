#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;

out VS_OUT {
    vec2 TexCoords;
} vs_out;

void main(){
    gl_Position = vec4(Position, 1.0);
    //vs_out.FragPos = vec3(model * vec4(position, 1.0f));
    vs_out.TexCoords = TexCoords;
}