#pragma once

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include "shader_program.h"
#include "entity.h"
#include "camera.h"

class Renderer {
private:
    int screenWidth;
    int screenHeight;
    SDL_Window* window;
    SDL_Renderer* renderer;

    ShaderProgram shaderProgram;
    GLuint VBO;
    GLuint IBO;
    GLuint VNBO;
    GLuint INBO;

    //tmp
    GLint vertexPos3DLocation;
    GLuint MVP;
    GLuint lightID;
    GLint vertexNormal;
    /*float theta = 0.f;
    std::vector<glm::vec4> meshCopy;
    GLfloat* vertexData;
    GLuint* indexData;
    std::vector<glm::vec3> normals;
    */

public:
    bool init();
    void render(Entity entity, Camera camera);
};
