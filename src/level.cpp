#include <SDL.h>

#include "level.h"
#include "resource_manager.h"
#include "render_manager.h"
#include "camera.h"

Level::Level(const std::string& guid) :
    guid_(guid),
    levelScript_(*this)
{
    //test_ = gResourceManager.getMesh("crate/crate.dae");

    // NOTE: Should create a factory method for Entity. static method in subClass?
    //testMonkey1_.setMesh(test_);
    //testMonkey1_.translate(glm::vec3(0.f, 0.f, -5.f));
    levelScript_.runScript(guid + "/init.lua");
}

Level::~Level() {

}
int Level::addEntity() {
    entities_.push_back(Entity());
    return entities_.size() - 1;
}
const Entity& Level::getEntity(int i) const {
    ASSERT(i >= 0 && i < static_cast<int>(entities_.size()));
    return entities_[i];
}
void Level::setEntity(int i, const Entity& entity) {
    ASSERT(i >= 0 && i <  static_cast<int>(entities_.size()));
    entities_[i] = entity;
}
void Level::events() {
    //Events
    // NOTE: Implement passing events back up the chain or something similar
    SDL_Event e;
    while (SDL_PollEvent(&e) == 1) {
        switch (e.type) {
        case SDL_QUIT:
            //running = false;
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
            case SDLK_w:
                camera_.translate(glm::vec3(0.f, 0.f, 0.1f));
                break;
            case SDLK_s:
                camera_.translate(glm::vec3(0.f, 0.f, -0.1f));
                break;
            case SDLK_a:
                camera_.translate(glm::vec3(0.1f, 0.f, 0.f));
                break;
            case SDLK_d:
                camera_.translate(glm::vec3(-0.1f, 0.f, 0.f));
                break;
            case SDLK_ESCAPE:
                //running = false;
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            camera_.rotate(glm::quat(glm::vec3(-(F32)e.motion.yrel / 400.f, -(F32)e.motion.xrel / 400.f, 0.f)));
            break;
        }
    }
}
void Level::logic() {
    //testMonkey1_.rotate(glm::quat(glm::vec3(0.00f, 0.03f, 0.00f)));
    levelScript_.runScript(guid_ + "/update.lua");
}
void Level::draw() const {
    //gRenderManager.render(testMonkey1_, camera_);
    for (const auto& entity : entities_) {
        gRenderManager.render(entity, camera_);
    }
}