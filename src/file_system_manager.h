#pragma once

#include <string>
#include <filesystem>

#include "common_imports.h"

using namespace std::tr2::sys;

struct RecursiveDirectoryRange {
    typedef recursive_directory_iterator iterator;
    RecursiveDirectoryRange(path p) : p_(p) {}

    iterator begin() { return recursive_directory_iterator(p_); }
    iterator end() { return recursive_directory_iterator(); }

    path p_;
};

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