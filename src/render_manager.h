#pragma once

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include "common_imports.h"
#include "shader_program.h"
#include "entity.h"
#include "camera.h"

class RenderManager {
private:
    S32 screenWidth_;
    S32 screenHeight_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    ShaderProgram shaderProgram_;
    GLuint vertexBufferHandle_;
    GLuint normalBufferHandle_;
    GLuint textureCoordsBufferHandle_;

    //tmp
    GLint vertexPos3DHandle_;
    GLint uvTexCoordsHandle_;
    GLint textureSamplerHandle_;
    GLint ModelViewProjectionHandle_;
    GLint ModelWorldProjectionHandle_;
    GLint vertexNormalHandle_;

public:
    explicit RenderManager();
    ~RenderManager();
    void init();
    void destroy();
    void clear();
    void render(Entity entity, Camera camera);
    void swap();
};

extern RenderManager gRenderManager;