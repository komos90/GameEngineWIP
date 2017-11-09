#version 330
in vec3 LVertexPos3D;
in vec3 lineColor;

uniform mat4 MVP;

out vec4 vertexColor;
void main() {
    gl_Position = MVP * vec4(LVertexPos3D.x, LVertexPos3D.y, LVertexPos3D.z, 1 );
    vertexColor = vec4(lineColor, 1.f);
}
