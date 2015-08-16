#include <fstream>

#include <SDL.h>

#include "util.h"

namespace util {
    std::string readFileToString(const std::string& filePath) {
        std::ifstream in(filePath, std::ios::in | std::ios::binary);
        if (!in) {
            SDL_Log("Error reading file to string.");
            return "";
        }
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize((unsigned int)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
}
