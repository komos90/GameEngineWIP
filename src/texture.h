#pragma once

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

class Texture {
private:
    GLuint textureId;
public:
    explicit Texture(GLuint textureId);
    ~Texture();

    GLuint getTextureId() const;
};