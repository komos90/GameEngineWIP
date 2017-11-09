#version 330
in vec4 vertexColor;
in vec2 fragmentUV;

uniform sampler2D textureSampler;

out vec4 LFragment;
void main() {
    vec2 flippedUV = vec2(fragmentUV.x, 1.0 - fragmentUV.y);
    //LFragment = vec4(1.0, 1.0, 1.0, 1.0);//vertexColor;
    LFragment = texture(textureSampler, flippedUV);
}
