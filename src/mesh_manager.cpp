#include "mesh_manager.h"
#include "common_imports.h"


// Global instance
MeshManager gMeshManager;


MeshManager::MeshManager() {
    ASSERT(this == &gMeshManager);
}
MeshManager::~MeshManager() {

}
void MeshManager::init() {

}
void MeshManager::destroy() {

}