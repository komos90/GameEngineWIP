#include <fstream>

#include <SDL.h>

#include "util.h"
#include <cstring>
#include <memory>

#include "common_imports.h"

namespace util {
    std::vector<std::string> splitString(const std::string& text, const char token) {
        const char* cStr = text.c_str();
        int start = 0;
        std::vector<std::string> stringList;
        int i = 0;
        while (cStr[i] != '\0') {
            i++;
            if (cStr[i] == token) {
                stringList.push_back(text.substr(start, i - start));
                i++;
                start = i;
            }
        }
        stringList.push_back(text.substr(start, text.size() - start));
        return stringList;
    }

    glm::vec3 bulletVecToGlmVec(const btVector3& vecIn) {
        return glm::vec3(vecIn.getX(), vecIn.getY(), vecIn.getZ());
    }
}