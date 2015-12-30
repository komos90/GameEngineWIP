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
    bool hasInit_;
    const std::string globalResourcesArchivePath_;

    std::map<std::string, Resource> resources_;
    std::vector<Mesh> meshes_;
    std::vector<Texture> textures_;

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