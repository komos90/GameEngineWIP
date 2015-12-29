#pragma once

#include <string>
#include <map>
#include <vector>

#include "resource.h"
#include "mesh.h"
#include "texture.h"

//TODO: Implement resource manager PROPERLY!
class ResourceManager {
private:
    bool hasInit;
    const std::string globalResourcesArchivePath;

    std::map<std::string, Resource> resources;
    std::vector<Mesh> meshes;
    std::vector<Texture> textures;

    void loadMesh(const std::string& guid);
    void loadTexture(const std::string& guid);
public:
    explicit ResourceManager();
    ~ResourceManager();
    void init();
    void destroy();

    void loadGlobalResources();
    const Mesh* getMesh(const std::string& guid);
    const Texture* getTexture(const std::string& guid);
};

extern ResourceManager gResourceManager;