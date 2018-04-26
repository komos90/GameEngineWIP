#include <SDL.h>
#include <stdio.h>

#include "shader_program.h"
#include "file_system_manager.h"
#include "util.h"

void ShaderProgram::init() {
    programId_ = glCreateProgram();
}
void ShaderProgram::loadVertexShader(const std::string& filePath) {
    std::string source = gFileSystemManager.readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        GLint maxLength = 1024;
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
        for (char c : errorLog) {
            if (c == '\0') { break; }
            fputc(c, stdout);
        }
        glDeleteShader(shaderId);
        ASSERT(false);
    }
    glAttachShader(programId_, shaderId);
}

void ShaderProgram::loadGeometryShader(const std::string& filePath) {
    std::string source = gFileSystemManager.readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        GLint maxLength = 1024;
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);
        for (char c : errorLog) {
            if (c == '\0') { break; }
            fputc(c, stdout);
        }
        glDeleteShader(shaderId);
        ASSERT(false);
    }
    glAttachShader(programId_, shaderId);
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
    glAttachShader(programId_, shaderId);
}
void ShaderProgram::linkProgram() {
    glLinkProgram(programId_);
    GLint programSuccess = GL_FALSE;
    glGetProgramiv(programId_, GL_LINK_STATUS, &programSuccess);
    ASSERT(programSuccess == GL_TRUE);
}
const GLuint ShaderProgram::getProgramId() const {
    return programId_;
}