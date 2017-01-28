#pragma once
#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

class Texture {
private:
    SDL_Surface* texture_;
public:
    explicit Texture(SDL_Surface* textureId);
    ~Texture();

    const SDL_Surface* getTexture() const;
};