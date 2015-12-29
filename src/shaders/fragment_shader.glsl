#version 140
in smooth vec4 vertexColor;
in vec2 fragmentUV;

uniform sampler2D textureSampler;

out vec4 LFragment;
void main() {
    vec2 flippedUV = vec2(fragmentUV.x, 1.0 - fragmentUV.y);
    //LFragment = vertexColor;
    LFragment = texture(textureSampler, flippedUV);
}
