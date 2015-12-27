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

Face::Face(S32 a, S32 b, S32 c) {
    vertexIndices[0] = a;
    vertexIndices[1] = b;
    vertexIndices[2] = c;
}
void Face::setNormal(glm::vec3 normal) {
    this->normal = normal;
}
S32 Face::getVertexIndexAt(S32 i) const {
    return vertexIndices[i];
}
const glm::vec3& Face::getNormal() const {
    return normal;
}

void Vertex::addFaceIndex(S32 i) {
    faces.push_back(i);
}
Vertex::Vertex() {
    vertex = glm::vec4();
}
Vertex::Vertex(glm::vec4 &vector) {
    vertex = vector;
}
const glm::vec4& Vertex::getVector() const {
    return vertex;
}
const std::vector<S32>& Vertex::getFaces() const {
    return faces;
}

const std::vector<Vertex>& Mesh::getVertices() const {
    return vertices;
}
const std::vector<Face>& Mesh::getFaces() const {
    return faces;
}
//TEMPORARY MESH FILE FORMAT
/*void Mesh::rawFileToMesh(std::string filePath) {
    char buf[512];
    std::fstream fs = std::fstream(filePath, std::ios_base::in);
    fs.getline(buf, 512);

    glm::vec4 vertices[3];
    S32 vertexIndices[3];
    std::string strBuf(buf);
    boost::char_separator<char> sep(" ");
    while (!fs.eof()) {

        strBuf = buf;
        boost::tokenizer<boost::char_separator<char> > tokens(strBuf, sep);

        S32 i = 0;
        for (const auto& t : tokens) {
            switch (i % 3)
            {
            case 0:
                vertices[i / 3].x = std::stof(t);
                break;
            case 1:
                vertices[i / 3].y = std::stof(t);
                break;
            case 2:
                vertices[i / 3].z = std::stof(t);
                vertices[i / 3].w = 1.f;

                vertexIndices[i / 3] = -1;
                for (S32 j = 0; j < (S32)this->vertices.size(); j++) {
                    if (this->vertices.at(j).getVector() == vertices[i / 3]) {
                        vertexIndices[i / 3] = j;
                        break;
                    }
                }
                if (vertexIndices[i / 3] == -1) {
                    this->vertices.push_back(Vertex(vertices[i / 3]));
                    vertexIndices[i / 3] = this->vertices.size() - 1;
                }
                this->vertices.at(vertexIndices[i / 3]).addFaceIndex(faces.size());
                break;
            }
            i++;
        }
        S32 s = this->vertices.size() - 1;
        faces.push_back(Face(vertexIndices[0], vertexIndices[1], vertexIndices[2]));

        faces.back().setNormal(glm::normalize(glm::cross(glm::vec3(this->vertices[vertexIndices[0]].getVector() - this->vertices[vertexIndices[1]].getVector()), glm::vec3(this->vertices[vertexIndices[0]].getVector() - this->vertices[vertexIndices[2]].getVector()))));

        fs.getline(buf, 512);
    }
}*/

void Mesh::daeFileToMesh(std::string filePath) {
    this->vertices.clear();
    this->faces.clear();
    pugi::xml_document doc;
    auto result = doc.load_file(filePath.c_str());
    ASSERT(result.status == pugi::status_ok);

    auto meshNode = doc.child("COLLADA").child("library_geometries").child("geometry").child("mesh");
    auto polyListNode = meshNode.child("polylist");
    auto posSrcLink = &(meshNode.child("vertices").child("input").attribute("source").as_string()[1]);
    auto normSrcLink = &(polyListNode.find_child_by_attribute("input", "semantic","NORMAL").attribute("source").as_string()[1]);
    auto posVerticesNode = meshNode.find_child_by_attribute("source", "id", posSrcLink).child("float_array");
    auto normVerticesNode = meshNode.find_child_by_attribute("source", "id", normSrcLink).child("float_array");
    auto verticesStr = posVerticesNode.first_child().value();
    auto normVerticesStr = normVerticesNode.first_child().value();
    auto indicesStr = polyListNode.child("p").first_child().value();

    auto verticesStrVec = util::splitString(verticesStr);
    auto normVerticesStrVec = util::splitString(normVerticesStr);
    auto indicesStrVec = util::splitString(indicesStr);

    std::vector<F32> verticesFloatVec(verticesStrVec.size());
    std::vector<F32> normVerticesFloatVec(normVerticesStrVec.size());
    std::vector<S32> indicesIntVec(indicesStrVec.size());
    std::transform(verticesStrVec.begin(), verticesStrVec.end(), verticesFloatVec.begin(), [](std::string text)->F32 {return stof(text); });
    std::transform(normVerticesStrVec.begin(), normVerticesStrVec.end(), normVerticesFloatVec.begin(), [](std::string text)->F32 {return stof(text); });
    std::transform(indicesStrVec.begin(), indicesStrVec.end(), indicesIntVec.begin(), [](std::string text)->S32 {return stoi(text); });
    
    std::vector<S32> posIndicesVec;
    std::vector<S32> normIndicesVec;
    for (auto i = size_t(0); i < indicesIntVec.size(); i++) {
        if (i % 2 == 0) {
            posIndicesVec.push_back(indicesIntVec[i]);
        } else {
            normIndicesVec.push_back(indicesIntVec[i]);
        }
    }
    
    for (auto i = size_t(0); i < verticesFloatVec.size(); i++) {
        if (i % 3 == 0) {
            this->vertices.push_back(Vertex(glm::vec4(verticesFloatVec[i], verticesFloatVec[i + 1], verticesFloatVec[i + 2], 0.f)));
        }
    }
    for (auto i = size_t(0); i < posIndicesVec.size(); i++) {
        if (i % 3 == 0) {
            auto face = Face(posIndicesVec[i], posIndicesVec[i + 1], posIndicesVec[i + 2]);
            face.setNormal(glm::vec3(normVerticesFloatVec[normIndicesVec[i]],
                                     normVerticesFloatVec[normIndicesVec[i + 1]],
                                     normVerticesFloatVec[normIndicesVec[i + 2]]));
            this->faces.push_back(face);
            this->vertices[posIndicesVec[i]].addFaceIndex(i / 3);
            this->vertices[posIndicesVec[i + 1]].addFaceIndex(i / 3);
            this->vertices[posIndicesVec[i + 2]].addFaceIndex(i / 3);
        }
    }
    int i = 0;
}