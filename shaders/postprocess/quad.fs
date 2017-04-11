#version 330 core
out vec4 colour;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

uniform mat4 inverseViewProjection;
uniform mat4 prevViewProjection;

uniform int alternativeView;

int nSamples = 24;
vec2 texCoords = fs_in.TexCoords;

void main(){
    float depth = texture(depthTexture, texCoords).r; // [1.0, 0.0]
    vec4 clipSpace = vec4(texCoords.x * 2 - 1, texCoords.y * 2 - 1, depth * 2 - 1, 1); //[-1.0, 1.0]
    vec4 worldPos = inverseViewProjection * clipSpace;
    worldPos /= worldPos.w;

    vec4 currentPos = clipSpace;
    vec4 previousPos = prevViewProjection * worldPos;
    previousPos /= previousPos.w;
    vec2 velocity = vec2(currentPos - previousPos);

    vec4 result = texture(screenTexture, texCoords);
    for(int i = 1; i < nSamples; ++i){
        vec2 offset = velocity * (float(i) / float(nSamples - 1) - 0.5);
        if((texCoords+offset).x >= 0.0f && (texCoords+offset).x <= 1.0f 
        && (texCoords+offset).y >= 0.0f && (texCoords+offset).y <= 1.0f){   //Can make sure it's not over sampling
            result += texture(screenTexture, texCoords+offset);
        }else{
            result += texture(screenTexture, texCoords);
        }
    }
    result /= nSamples;
    // float f=100.0;
    // float n = 0.1;
    // float z = (2 * n) / (f + n - texture( screenTexture, texCoords ).r * (f - n));

    // colour = vec4(vec3(z), 1.0f);
    colour = result;
}