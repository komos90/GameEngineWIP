#version 330
in vec4 vertexColor;

out vec4 LFragment;
void main() {
    LFragment = vertexColor;
}
