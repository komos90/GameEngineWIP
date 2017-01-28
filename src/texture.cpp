#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include "texture.h"

//NOTE: Should the texture load itself?
//      How should I hide this explicit opengl stuff?
Texture::Texture(SDL_Surface* texIn):
    texture_(texIn)
{}

Texture::~Texture() {
    //HACK
    //SDL_FreeSurface(texture_);
}

const SDL_Surface* Texture::getTexture() const {
    return texture_;
}