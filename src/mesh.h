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
    S32 vertexIndices[3];
    
    glm::vec3 normal;
public:
    // NOTE: make private + getters/setters
    S32 uvIndices[3];
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
    // NOTE: Should wrap opengl stuff?
    // NOTE: Mesh can't be a POD if it uses std::vector?
    // NOTE: Mesh shouldn't be POD!
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    // NOTE: Should this be encapuslated/ use a class instead of glm::vec2
    std::vector<glm::vec2> uvTexCoords;
    // NOTE: Add getters/setters and generally decide on the best way to arrange mesh class.
    const Texture* texture;
public:
    mutable GLuint vboId;
    mutable GLuint iboId;
    mutable GLuint uvboId;
    mutable GLuint uviboId;
    mutable GLuint vnboId;

    explicit Mesh();
    //Should be in gMeshManager
    void daeFileToMesh(std::string filePath);
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Face>& getFaces() const;
    const std::vector<glm::vec2>& getUvTexCoords() const; //NOTE: needs a better name
    const Texture* getTexture() const;
};

