#pragma once

#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "common_imports.h"

class Face {
private:
    S32 vertexIndices[3];
    glm::vec3 normal;
public:
    explicit Face(S32 a, S32 b, S32 c);
    void setNormal(glm::vec3 normal);
    S32 Face::getVertexIndexAt(S32 i) const;
    const glm::vec3& Face::getNormal() const;
};

class Vertex {
private:
    glm::vec4 vertex;
    std::vector<S32> faces;
public:
    explicit Vertex();
    explicit Vertex(glm::vec4 &vector);
    void addFaceIndex(S32 i);
    const glm::vec4& getVector() const;
    const std::vector<S32>& getFaces() const;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
public:
    //Should be in gMeshManager
    void daeFileToMesh(std::string filePath);
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Face>& getFaces() const;
};
