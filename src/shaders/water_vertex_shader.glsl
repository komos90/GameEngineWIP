#version 330

in vec4 LVertexPos3D;

uniform mat4 MVP;
uniform float time;

void main() {
    vec4 wavePos;
    float x = LVertexPos3D.x;
    float y = LVertexPos3D.y;
    float height = 2.0;
    wavePos = LVertexPos3D + vec4( 0.0, 0.0, sin(x + time) * cos(y + time), 0.0);
    gl_Position = MVP * vec4(wavePos);
}
