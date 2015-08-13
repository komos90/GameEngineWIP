#include <SDL.h>

#include "shader_program.h"
#include "util.h"

void ShaderProgram::init() {
    programId = glCreateProgram();
}
void ShaderProgram::loadVertexShader(const std::string& filePath) {
    std::string source = util::readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        SDL_Log("Vertex shader compilation error.");
    }
    glAttachShader(programId, shaderId);
}
void ShaderProgram::loadFragmentShader(const std::string& filePath) {
    std::string source = util::readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        SDL_Log("Fragment shader compilation error.");
    }
    glAttachShader(programId, shaderId);
}
void ShaderProgram::linkProgram() {
    glLinkProgram(programId);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        SDL_Log("Error linking program");
    }
}
const GLuint ShaderProgram::getProgramId() const {
    return programId;
}