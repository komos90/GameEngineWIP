#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "mesh.h"

Face::Face(int a, int b, int c) {
    vertexIndices[0] = a;
    vertexIndices[1] = b;
    vertexIndices[2] = c;
}
void Face::setNormal(glm::vec3 normal) {
    this->normal = normal;
}
int Face::getVertexIndexAt(int i) const {
    return vertexIndices[i];
}
const glm::vec3& Face::getNormal() const {
    return normal;
}

void Vertex::addFaceIndex(int i) {
    faces.push_back(i);
}
Vertex::Vertex(glm::vec4 &vector) {
    vertex = vector;
}
const glm::vec4& Vertex::getVector() const {
    return vertex;
}
const std::vector<int>& Vertex::getFaces() const {
    return faces;
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return vertices;
}
const std::vector<Face>& Mesh::getFaces() const {
    return faces;
}
