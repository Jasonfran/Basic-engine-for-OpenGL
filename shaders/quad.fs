#version 330 core
out vec4 colour;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 prevProjection;
uniform mat4 prevView;

int nSamples = 32;
vec2 texCoords = fs_in.TexCoords;

void main(){
    float depth = texture(depthTexture, texCoords).r; // [1.0, 0.0]
    vec4 viewportPos = vec4(texCoords.x * 2 - 1, (1 - texCoords.y) * 2 - 1, depth, 1);
    vec4 D = inverse(projection * view) * viewportPos;
    vec4 worldPos = D / D.w;

    vec4 currentPos = viewportPos;
    vec4 previousPos = prevProjection * prevView * worldPos;
    previousPos /= previousPos.w;
    vec2 velocity = vec2(currentPos - previousPos);

    vec4 result = texture(screenTexture, texCoords);
    for(int i = 1; i < nSamples; ++i){
        vec2 offset = velocity * (float(i) / float(nSamples - 1) - 0.5);
        result += texture(screenTexture, texCoords+offset);
    }
    result /= float(nSamples);
    colour = result;

}