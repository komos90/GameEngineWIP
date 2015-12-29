#pragma once

#include <string>

#include "common_imports.h"

class FileSystemManager {
private:
public:
    explicit FileSystemManager();
    ~FileSystemManager();
    void init();
    void destroy();
    
    bool syncRead(const std::string& filePath, U8* buffer, size_t bufferSize, size_t& bytesRead);
    std::string readFileToString(const std::string& filePath);
};

extern FileSystemManager gFileSystemManager;