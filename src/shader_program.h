#pragma once

#include <string>

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

class ShaderProgram {
private:
    GLuint programId_;
public:
    void init();
    void loadVertexShader(const std::string& filePath);
    //NOTE: Implement or delete?
    void loadGeometryShader(const std::string& filePath);
    void loadFragmentShader(const std::string& filePath);
    void linkProgram();
    const GLuint getProgramId() const;
};