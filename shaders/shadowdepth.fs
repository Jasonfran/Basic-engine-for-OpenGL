#version 330 core

in vec4 fragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main(){
    // Calculate linear depth
    float lightDist = length(fragPos.xyz - lightPos);
    // Need in range [0;1]
    lightDist /= far_plane;
    gl_FragDepth = lightDist;
}