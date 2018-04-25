#pragma once

#include <string>
#include <map>
#include <vector>
#include <stack>

#include "resource.h"
#include "mesh.h"
#include "texture.h"
#include "resource_array.h"

//TODO: Implement resource manager PROPERLY!
class ResourceManager {
private:
    bool hasInit_;
    const std::string globalResourcesArchivePath_;
    const std::string resourceBasePath_;

    std::map<std::string, Resource> resources_;
    std::stack<ResourceArray<Mesh>> meshes_;
    std::stack<ResourceArray<Texture>> textures_;

    void loadMesh(const std::string& guid);
    void loadTexture(const std::string& guid);
public:
    explicit ResourceManager();
    ~ResourceManager();
    void init();
    void destroy();

    void loadGlobalResources();
    const Mesh* getMesh(const std::string& guid);
    const Texture& getTexture(const std::string& guid);
};

extern ResourceManager gResourceManager;