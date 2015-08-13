#pragma once

#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Face {
private:
    int vertexIndices[3];
    glm::vec3 normal;
public:
    explicit Face(int a, int b, int c);
    void setNormal(glm::vec3 normal);
    int Face::getVertexIndexAt(int i) const;
    const glm::vec3& Face::getNormal() const;
};

class Vertex {
private:
    glm::vec4 vertex;
    std::vector<int> faces;
public:
    explicit Vertex(glm::vec4 &vector);
    void addFaceIndex(int i);
    const glm::vec4& getVector() const;
    const std::vector<int>& getFaces() const;
};

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
public:
    void rawFileToMesh(std::string filePath);
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Face>& getFaces() const;
};

