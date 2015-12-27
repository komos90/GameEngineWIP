#pragma once

#include <string>
#include <vector>

namespace util {
    std::string readFileToString(const std::string& filePath);
    std::vector<std::string> splitString(const std::string& text, const char tokens=' ');
}
