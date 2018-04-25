#pragma once

#include "common_imports.h"

#include<array>

#include "texture.h"

struct SkyBox {
    const SDL_Surface* pos_x_tex_;
    const SDL_Surface* pos_y_tex_;
    const SDL_Surface* pos_z_tex_;
    const SDL_Surface* neg_x_tex_;
    const SDL_Surface* neg_y_tex_;
    const SDL_Surface* neg_z_tex_;
    
    std::array<F32, 24> cube_vertices_;
    std::array<GLuint, 24> cube_indices_;
    
    explicit SkyBox();
    void init(const std::string& skyBoxPath, float scale);
};
