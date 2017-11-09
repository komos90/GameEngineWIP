#include <string>
#include <fstream>

#include "file_system_manager.h"
#include "common_imports.h"


// Global instance
FileSystemManager gFileSystemManager;


FileSystemManager::FileSystemManager() {
    ASSERT(this == &gFileSystemManager);
}
FileSystemManager::~FileSystemManager() {

}
void FileSystemManager::init() {

}
void FileSystemManager::destroy() {

}

bool syncRead(const std::string& filePath, U8* buffer, size_t bufferSize, size_t& bytesRead) {
    return false;
}

std::string FileSystemManager::readFileToString(const std::string& filePath) {
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    printf(filePath.c_str());
    ASSERT(in);

    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize((unsigned int)in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return contents;
}
