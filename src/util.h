#pragma once

#include <string>
#include <vector>

#include "glm/vec3.hpp"
#include "LinearMath/btVector3.h"

namespace util {
    std::vector<std::string> splitString(const std::string& text, const char tokens=' ');
    glm::vec3 bulletVecToGlmVec(const btVector3& vecIn);
}
