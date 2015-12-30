#include <vector>
#include <fstream>
#include <iostream>
#include <memory>
#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
//#include <boost\tokenizer.hpp>
#include <pugixml.hpp>

#include "common_imports.h"
#include "mesh.h"
#include "util.h"
#include "resource_manager.h"

Face::Face(S32 a, S32 b, S32 c) {
    vertexIndices_[0] = a;
    vertexIndices_[1] = b;
    vertexIndices_[2] = c;
}
void Face::setUvIndexAt(S32 i, S32 uvIndex) {
    ASSERT(i >= 0 && i < 3);
    uvIndices_[i] = uvIndex;
}
S32 Face::getVertexIndexAt(S32 i) const {
    ASSERT(i >= 0 && i < 3);
    return vertexIndices_[i];
}
S32 Face::getUvIndexAt(S32 i) const {
    ASSERT(i >= 0 && i < 3);
    return uvIndices_[i];
}

void Vertex::addFaceIndex(S32 i) {
    faces_.push_back(i);
}
Vertex::Vertex() {
    vertex_ = glm::vec4();
}
Vertex::Vertex(glm::vec4 &vector) {
    vertex_ = vector;
}
void Vertex::setNormal(const glm::vec3& normal) {
    normal_ = normal;
}

const glm::vec4& Vertex::getVector() const {
    return vertex_;
}
const std::vector<S32>& Vertex::getFaces() const {
    return faces_;
}
const glm::vec3& Vertex::getNormal() const {
    return normal_;
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return vertices_;
}
const std::vector<Face>& Mesh::getFaces() const {
    return faces_;
}

Mesh::Mesh() :
    vboId_(-1),
    iboId_(-1),
    uvboId_(-1),
    vnboId_(-1)
{

}

void Mesh::daeFileToMesh(std::string filePath) {
    vertices_.clear();
    faces_.clear();
    pugi::xml_document doc;
    auto result = doc.load_file(filePath.c_str());
    ASSERT(result.status == pugi::status_ok);

    auto texturePath = doc.child("COLLADA").child("library_images").child("image").child("init_from").first_child().value();
    texture_ = gResourceManager.getTexture(texturePath);

    auto meshNode = doc.child("COLLADA").child("library_geometries").child("geometry").child("mesh");
    auto polyListNode = meshNode.child("polylist");
    auto posSrcLink = &(meshNode.child("vertices").child("input").attribute("source").as_string()[1]);
    auto normSrcLink = &(polyListNode.find_child_by_attribute("input", "semantic","NORMAL").attribute("source").as_string()[1]);
    auto texSrcLink = &(polyListNode.find_child_by_attribute("input", "semantic", "TEXCOORD").attribute("source").as_string()[1]);
    auto posVerticesNode = meshNode.find_child_by_attribute("source", "id", posSrcLink).child("float_array");
    auto normVerticesNode = meshNode.find_child_by_attribute("source", "id", normSrcLink).child("float_array");
    auto texVerticesNode = meshNode.find_child_by_attribute("source", "id", texSrcLink).child("float_array");
    auto verticesStr = posVerticesNode.first_child().value();
    auto normVerticesStr = normVerticesNode.first_child().value();
    auto texVerticesStr = texVerticesNode.first_child().value();
    auto indicesStr = polyListNode.child("p").first_child().value();

    auto verticesStrVec = util::splitString(verticesStr);
    auto normVerticesStrVec = util::splitString(normVerticesStr);
    auto texVerticesStrVec = util::splitString(texVerticesStr);
    auto indicesStrVec = util::splitString(indicesStr);

    std::vector<F32> verticesFloatVec(verticesStrVec.size());
    std::vector<F32> normVerticesFloatVec(normVerticesStrVec.size());
    std::vector<F32> texVerticesFloatVec(texVerticesStrVec.size());
    std::vector<S32> indicesIntVec(indicesStrVec.size());
    std::transform(verticesStrVec.begin(), verticesStrVec.end(), verticesFloatVec.begin(), [](std::string text)->F32 {return stof(text); });
    std::transform(normVerticesStrVec.begin(), normVerticesStrVec.end(), normVerticesFloatVec.begin(), [](std::string text)->F32 {return stof(text); });
    std::transform(texVerticesStrVec.begin(), texVerticesStrVec.end(), texVerticesFloatVec.begin(), [](std::string text)->F32 {return stof(text); });
    std::transform(indicesStrVec.begin(), indicesStrVec.end(), indicesIntVec.begin(), [](std::string text)->S32 {return stoi(text); });
    
    std::vector<S32> posIndicesVec;
    std::vector<S32> normIndicesVec;
    std::vector<S32> texIndicesVec;
    for (auto i = size_t(0); i < indicesIntVec.size(); i++) {
        if (i % 3 == 0) {
            posIndicesVec.push_back(indicesIntVec[i]);
        } else if (i % 3 == 1) {
            normIndicesVec.push_back(indicesIntVec[i]);
        } else {
            texIndicesVec.push_back(indicesIntVec[i]);
        }
    }
    
    for (auto i = size_t(0); i < verticesFloatVec.size(); i++) {
        if (i % 3 == 0) {
            vertices_.push_back(Vertex(glm::vec4(verticesFloatVec[i], verticesFloatVec[i + 1], verticesFloatVec[i + 2], 0.f)));
        }
    }
    for (auto i = size_t(0); i < texVerticesFloatVec.size(); i++) {
        if (i % 2 == 0) {
            textureCoords_.push_back(glm::vec2(texVerticesFloatVec[i], texVerticesFloatVec[i + 1]));
        }
    }
    for (auto i = size_t(0); i < posIndicesVec.size(); i++) {
        if (i % 3 == 0) {
            auto face = Face(posIndicesVec[i], posIndicesVec[i + 1], posIndicesVec[i + 2]);
            face.setUvIndexAt(0, texIndicesVec[i]);
            face.setUvIndexAt(1, texIndicesVec[i + 1]);
            face.setUvIndexAt(2, texIndicesVec[i + 2]);
            /*face.setNormal(glm::vec3(normVerticesFloatVec[normIndicesVec[i]],
                                     normVerticesFloatVec[normIndicesVec[i + 1]],
                                     normVerticesFloatVec[normIndicesVec[i + 2]]));*/
            for (auto j = i; j < i + 3; j++){
                vertices_[posIndicesVec[i]].setNormal(glm::vec3(normVerticesFloatVec[normIndicesVec[i] / 3], texVerticesFloatVec[texIndicesVec[i] / 3 + 1], texVerticesFloatVec[texIndicesVec[i] / 3 + 2]));
            }
            faces_.push_back(face);
            vertices_[posIndicesVec[i]].addFaceIndex(i / 3);
            vertices_[posIndicesVec[i + 1]].addFaceIndex(i / 3);
            vertices_[posIndicesVec[i + 2]].addFaceIndex(i / 3);
        }
    }
}

const Texture* Mesh::getTexture() const {
    return texture_;
}

const std::vector<glm::vec2>& Mesh::getTextureCoords() const {
    return textureCoords_;
}