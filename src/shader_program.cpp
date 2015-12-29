#include <SDL.h>

#include "shader_program.h"
#include "file_system_manager.h"
#include "util.h"

void ShaderProgram::init() {
    programId = glCreateProgram();
}
void ShaderProgram::loadVertexShader(const std::string& filePath) {
    std::string source = gFileSystemManager.readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    ASSERT(didShaderCompile == GL_TRUE);
    glAttachShader(programId, shaderId);
}
void ShaderProgram::loadFragmentShader(const std::string& filePath) {
    std::string source = gFileSystemManager.readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    ASSERT(didShaderCompile == GL_TRUE);
    glAttachShader(programId, shaderId);
}
void ShaderProgram::linkProgram() {
    glLinkProgram(programId);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
    ASSERT(programSuccess == GL_TRUE);
}
const GLuint ShaderProgram::getProgramId() const {
    return programId;
}