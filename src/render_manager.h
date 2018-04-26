#pragma once

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include "common_imports.h"
#include "shader_program.h"
#include "entity.h"
#include "camera.h"
#include "debug_draw.h"
#include "prof_timer.h"

class RenderManager {
private:
    S32 screenWidth_;
    S32 screenHeight_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    ShaderProgram shaderProgram_;
    ShaderProgram debugLinesShader_;
    ShaderProgram waterShader_;

    //tmp
    GLint vertexPos3DHandle_;
    GLint uvTexCoordsHandle_;
    GLint textureSamplerHandle_;
    GLint ModelViewProjectionHandle_;
    GLint ModelWorldProjectionHandle_;
    GLint vertexNormalHandle_;
    //ProfTimer timer{ "RENDER", 60 };

    //Shoud be in debugdraw
    std::vector<GLuint> debugLineHandles_;
public:
    explicit RenderManager();
    ~RenderManager();
    void init();
    void destroy();
    void clear();
    void render(const Entity& entity, const Camera& camera);
    void renderDebug(DebugDraw& dDraw, const Camera& camera);
    void renderWaterLevel(const Camera& camera);
    void swap();
};

extern RenderManager gRenderManager;