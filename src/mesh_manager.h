#pragma once

#include <string>

class MeshManager {
private:
public:
    explicit MeshManager();
    ~MeshManager();
    void init();
    void destroy();
};

extern MeshManager gMeshManager;