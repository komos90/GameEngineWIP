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

    const Texture* texture_;
public:
    // NOTE: Should wrap opengl stuff?
    // handles for mesh data held in GPU
    mutable GLuint vboId_;
    mutable GLuint iboId_;
    mutable GLuint uvboId_;
    mutable GLuint uviboId_;
    mutable GLuint vnboId_;

    explicit Mesh();
    // NOTE: Should be in gResourceManager
    void daeFileToMesh(std::string filePath);
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Face>& getFaces() const;
    const std::vector<glm::vec2>& getTextureCoords() const;
    const Texture* getTexture() const;
};

