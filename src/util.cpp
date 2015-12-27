#include <fstream>

#include <SDL.h>

#include "util.h"
#include <cstring>
#include <memory>

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
}