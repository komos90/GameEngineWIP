#version 140
in vec4 LVertexPos3D;
in vec3 normals;
in vec2 vertexUV;
uniform mat4 MVP;
uniform mat4 MWP;
out vec4 vertexColor;
out vec2 fragmentUV;

void main() {
    gl_Position = MVP * vec4(LVertexPos3D.x, LVertexPos3D.y, LVertexPos3D.z, 1 );
    mat4 normalMWP = transpose(inverse(MWP));
    vec3 newNormal = normalize(vec3(normalMWP * vec4(normals, 0.f)));
    fragmentUV = vertexUV;
    vertexColor = vec4(1.f, 1.f, 1.f, 1.f);
    //vertexColor = ((dot(light, newNormal) + 1.f) / 2.f) * vec4(0.5f, 0.2f, 0.0f, 1.f) + (1 - ((dot(light, newNormal) + 1.f) / 2.f)) * vec4(0.0f, 0.05f, 0.1f, 1.f);
}
