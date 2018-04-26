#pragma once

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include "common_imports.h"
#include "texture.h"

class Face {
private:
    S32 vertexIndices_[3];
    S32 uvIndices_[3];
public:
    explicit Face(S32 a, S32 b, S32 c);
    void Face::setUvIndexAt(S32 i, S32 uvIndex);
    S32 Face::getVertexIndexAt(S32 i) const;
    S32 Face::getUvIndexAt(S32 i) const;
};

class Vertex {
private:
    glm::vec4 vertex_;
    glm::vec3 normal_;
    std::vector<S32> faces_;
public:
    explicit Vertex();
    explicit Vertex(glm::vec4 &vector);
    void addFaceIndex(S32 i);
    void setNormal(const glm::vec3& normal);
    const glm::vec4& getVector() const;
    const std::vector<S32>& getFaces() const;
    const glm::vec3& getNormal() const;
};

class Mesh {
private:
    std::vector<Vertex> vertices_;
    std::vector<glm::vec2> textureCoords_;
    std::vector<Face> faces_;

    //Render ready data
    // THis is duplication & should really be the norma way it's stored, with Face & Vertex being views onto the data.
    std::vector<glm::vec4> vertexData_;
    std::vector<glm::vec2> uvData_;
    std::vector<glm::vec3> normalData_;

    std::string texture_guid_;
public:
    // NOTE: Should wrap opengl stuff?
    // handles for mesh data held in GPU
    // NOTE: I don't think I'm using these apart from texId_, should
    //       get these working.
    mutable GLuint vboId_;
    //mutable GLuint iboId_;
    mutable GLuint uvboId_;
    //mutable GLuint uviboId_;
    mutable GLuint vnboId_;
    mutable GLuint texId_;

    const std::vector<glm::vec4>& vertexData() const;
    const std::vector<glm::vec2>& uvData() const;
    const std::vector<glm::vec3>& normalData() const;

    explicit Mesh();
    Mesh(Mesh&& other);
    Mesh& operator=(Mesh&& other);
    // NOTE: Should be in gResourceManager
    void daeFileToMesh(std::string filePath);
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Face>& getFaces() const;
    const std::vector<glm::vec2>& getTextureCoords() const;
    const Texture& getTexture() const;
};

