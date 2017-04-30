#version 330 core
layout (triangles) in; // triangles from vertex shader
layout (triangle_strip, max_vertices=18) out; // one triangle for each view, 6 * 3 = 18

uniform mat4 shadowTransforms[6]; // to transform a vertex position into the shadow space

out vec4 fragPos; // The fragment position in world space

void main(){
    for(int face = 0; face < 6; face++){
        gl_Layer = face; // each layer represents a face of the cubemap
        for(int i = 0; i < 3; i++){ // each triangle vertex
            fragPos = gl_in[i].gl_Position; // world space vertex position
            gl_Position = shadowTransforms[face] * fragPos; // move this vertex into the shadow space for this face
            EmitVertex(); // Add to vertex stream
        }
        EndPrimitive(); // 3 vertices from vertex stream. Turn into primitive and start new one.
    }
}