
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include "texture.h"

//NOTE: Should the texture load itself?
//      How should I hide this explicit opengl stuff?
Texture::Texture(GLuint textureId):
    textureId(textureId)
{

}

Texture::~Texture() {

}

GLuint Texture::getTextureId() const {
    return textureId;
}