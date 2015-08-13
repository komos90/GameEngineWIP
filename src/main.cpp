#include <vector>
#include <fstream>

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include <glm/geometric.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <boost\tokenizer.hpp>

#include "mesh.h"
#include "game.h"

//Globals
const int FPS_CAP = 60;

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
                for (int j = 0; j < this->vertices.size(); j++) {
                    if (this->vertices.at(j).getVector() == vertices[i / 3]) {
                        vertexIndices[i / 3] = j;
                        break;
                    }
                }
                if (vertexIndices[i / 3] == -1) {
                    this->vertices.push_back(Vertex(vertices[i / 3]));
                    vertexIndices[i / 3] = this->vertices.size() - 1;
                }
                this->vertices.at(vertexIndices[i/3]).addFaceIndex(faces.size());
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




int main(int argc, char* argv[]) {

    Game game;
    if (!game.init()) {
        SDL_Log("Game initialisation failed.");
    }

    float theta = 0.01f;
    bool running = true;
    while (running) {
        //Manage fps
        int frameStartTime = SDL_GetTicks();

        game.events();
        game.logic();
        game.draw();

        if (!game.isRunning()) running = false;
        //Manage fps
        int frameDelta = SDL_GetTicks() - frameStartTime;
        if (frameDelta < 1000 / FPS_CAP) {
            SDL_Delay(1000 / FPS_CAP - frameDelta);
        }
    }
	return 0;
}