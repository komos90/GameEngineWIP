#include <string>

#include "sky_box.h"
#include "resource_manager.h"

SkyBox::SkyBox() {
    cube_vertices_ = {
        -1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
    };

    cube_indices_ = {
        0, 1, 2, 3,
        3, 2, 6, 7,
        7, 6, 5, 4,
        4, 5, 1, 0,
        0, 3, 7, 4,
        1, 2, 6, 5,
    };
}

void SkyBox::init(const std::string& skyBoxPath, float scale) {
    pos_x_tex_ = gResourceManager.getTexture(skyBoxPath + "/pos_x.png").getTexture();
    pos_y_tex_ = gResourceManager.getTexture(skyBoxPath + "/pos_y.png").getTexture();
    pos_z_tex_ = gResourceManager.getTexture(skyBoxPath + "/pos_z.png").getTexture();
    neg_x_tex_ = gResourceManager.getTexture(skyBoxPath + "/neg_x.png").getTexture();
    neg_y_tex_ = gResourceManager.getTexture(skyBoxPath + "/neg_y.png").getTexture();
    neg_z_tex_ = gResourceManager.getTexture(skyBoxPath + "/neg_z.png").getTexture();
 
    for (auto& elem : cube_vertices_) {
        elem *= scale;
    }
}