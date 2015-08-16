#include <vector>
#include <fstream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <boost\tokenizer.hpp>

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
//TEMPORARY MESH FILE FORMAT
void Mesh::rawFileToMesh(std::string filePath) {
    char buf[512];
    std::fstream fs = std::fstream(filePath, std::ios_base::in);
    fs.getline(buf, 512);

    glm::vec4 vertices[3];
    int vertexIndices[3];
    std::string strBuf(buf);
    boost::char_separator<char> sep(" ");
    while (!fs.eof()) {

        strBuf = buf;
        boost::tokenizer<boost::char_separator<char> > tokens(strBuf, sep);

        int i = 0;
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
                for (int j = 0; j < (int)this->vertices.size(); j++) {
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
        int s = this->vertices.size() - 1;
        faces.push_back(Face(vertexIndices[0], vertexIndices[1], vertexIndices[2]));

        faces.back().setNormal(glm::normalize(glm::cross(glm::vec3(this->vertices[vertexIndices[0]].getVector() - this->vertices[vertexIndices[1]].getVector()), glm::vec3(this->vertices[vertexIndices[0]].getVector() - this->vertices[vertexIndices[2]].getVector()))));

        fs.getline(buf, 512);
    }
}
