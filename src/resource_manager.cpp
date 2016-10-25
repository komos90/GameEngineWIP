#include <filesystem>

#include <SDL.h>
#include <SDL_image.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

#include "resource_manager.h"
#include "common_imports.h"
#include "file_system_manager.h"

using namespace std::tr2::sys;

// Global instance
ResourceManager gResourceManager;


void ResourceManager::loadMesh(const std::string& guid) {
    ASSERT(resources_[guid].type == ResourceType::MESH);

    meshes_.push_back(Mesh());
    //NOTE: Resource manager should  do the loading of dae file
    //NOTE: Should have a base path for resources
    meshes_[meshes_.size() - 1].daeFileToMesh(resourceBasePath_ + guid);
    resources_[guid].handle = meshes_.size() - 1;
    resources_[guid].refCount++;
}

void ResourceManager::loadTexture(const std::string& guid) {
    //NOTE: SDL should be wrapped or replaced.
    SDL_Surface* texSurf = IMG_Load((resourceBasePath_ + guid).c_str());
    //NOTE OpenGL should be wrapped or done in different class
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSurf->w, texSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texSurf->pixels);
    ASSERT(glGetError() == GL_NO_ERROR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(texSurf);

    textures_.push_back(Texture(texId));
    resources_[guid].handle = textures_.size() - 1;
    resources_[guid].refCount++;
}

ResourceManager::ResourceManager() :
    //NOTE: Should have defines in a file for BASE_PATH, RES_PATH etc.
    hasInit_(false),
    globalResourcesArchivePath_("res/global_assets.zip"),
    resourceBasePath_("res/engine_ready/")
{
    ASSERT(this == &gResourceManager);
}
ResourceManager::~ResourceManager() {

}
void ResourceManager::init() {
    hasInit_ = true;
}
void ResourceManager::destroy() {
    
}

void ResourceManager::loadGlobalResources() {
    ASSERT(hasInit_);
    for (auto it : RecursiveDirectoryRange(resourceBasePath_)) {
        auto fullPath = it.path().generic_string();
        auto shortPath = fullPath.substr(resourceBasePath_.size(), -1);
        if (it.path().extension() == ".dae") {
            Resource meshResource;
            meshResource.type = ResourceType::MESH;
            resources_[shortPath] = meshResource;
        } else if (it.path().extension() == ".png") {
            Resource texResource;
            texResource.type = ResourceType::TEXTURE;
            resources_[shortPath] = texResource;
        }
    }

    for (auto it : RecursiveDirectoryRange(resourceBasePath_ + "global/")) {
        auto fullPath = it.path().generic_string();
        auto shortPath = fullPath.substr(resourceBasePath_.size(), -1);
        if (resources_.find(shortPath) != resources_.end()) {
            switch (resources_[shortPath].type) {
            case ResourceType::MESH:
                loadMesh(shortPath);
                break;
            case ResourceType::TEXTURE:
                loadTexture(shortPath);
                break;
            default:
                ASSERT(false);
                break;
            }
        }
    }
}

const Mesh* ResourceManager::getMesh(const std::string& guid) {
    ASSERT(hasInit_);
    //NOTE: Maybe it's silly to have a resources Map, instead should have seperate mesh map, tex map etc.
    //NOTE: Meshes should probably be cached in the renderer.So store a GLuint id as well as mesh data in Mesh.
    ASSERT(resources_.find(guid) != resources_.end());
    return &meshes_[resources_[guid].handle];
}

const Texture* ResourceManager::getTexture(const std::string& guid) {
    ASSERT(hasInit_);
    ASSERT(resources_.find(guid) != resources_.end());
    if (resources_[guid].handle == -1) {
        loadTexture(guid);
    }
    return &textures_[resources_[guid].handle];
}