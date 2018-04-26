#include <SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "level.h"
#include "resource_manager.h"
#include "render_manager.h"
#include "camera.h"
#include "debug_draw.h"
#include "util.h"
#include "game.h"

Level::Level(const std::string& guid, Game& game) :
    guid_(guid),
    levelScript_(*this), 
    game_(game)
{
    // START: Init Bullet3 physics world
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    collisionWorld_ = new btCollisionWorld(dispatcher, overlappingPairCache, collisionConfiguration);
    collisionWorld_->setDebugDrawer(&debugDraw_);
    // END: Init Bullet3 physics world

    levelScript_.runScript(guid + "/init.lua");
    ASSERT(player_ != nullptr);
    camera_.getTransform().setPosition(glm::vec3(0.f, 0.f, 5.f));
}

Level::~Level() {

}
void Level::setPlayerEntity(U32 entityId) {
    player_ = &entities_[entityId];
}
int Level::addEntity() {
    entities_.push_back(Entity());
    auto &justAdded = entities_.back();
    justAdded.setupCollisionObject();
    auto colObjAddr = justAdded.getCollisionObject();
    collisionWorld_->addCollisionObject(colObjAddr);
    return entities_.size() - 1;
}
const Entity& Level::getEntity(int i) const {
    ASSERT(i >= 0 && i < static_cast<int>(entities_.size()));
    return entities_[i];
}
Entity& Level::getEntity(int i) {
    ASSERT(i >= 0 && i < static_cast<int>(entities_.size()));
    return entities_[i];
}
void Level::setEntity(int i, const Entity& entity) {
    ASSERT(i >= 0 && i <  static_cast<int>(entities_.size()));
    // NOTE: This using copy assignment??
    //entities_[i] = entity;
}
void Level::events() {
    // Events
    // NOTE: Implement passing events back up the chain or something similar
    SDL_Event e;
    while (SDL_PollEvent(&e) == 1) {
        switch (e.type) {
            case SDL_QUIT: {
                game_.stopRunning();
                break;
            }
            case SDL_KEYDOWN: {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_.stopRunning();
                    break;
                }
                break;
            }
            case SDL_MOUSEMOTION: {
                camera_.rotate(glm::quat(glm::vec3(-(F32)e.motion.yrel / 400.f, -(F32)e.motion.xrel / 400.f, 0.f)));
                break;
            }
        }
    }

    // Handle keyboard movement
    //--------------------------
    auto keys = SDL_GetKeyboardState(NULL);

    { //Player movement
        auto moveVec = glm::vec3(0.f, 0.f, 0.f);
        F32 moveSpeed = 0.3f;
        if (keys[SDL_SCANCODE_W]) {
            moveVec += glm::vec3(0.f, 0.f, -moveSpeed);
        }
        if (keys[SDL_SCANCODE_S]) {
            moveVec += glm::vec3(0.f, 0.f, moveSpeed);
        }
        if (keys[SDL_SCANCODE_A]) {
            moveVec += glm::vec3(-moveSpeed, 0.f, 0.f);
        }
        if (keys[SDL_SCANCODE_D]) {
            moveVec += glm::vec3(moveSpeed, 0.f, 0.f);
        }
        auto rot4Mat = glm::mat4_cast(camera_.getTransform().getOrientation());
        auto rot3Mat = glm::mat3(rot4Mat);
        rot3Mat[2][2] = 0.f;
        moveVec = rot3Mat * moveVec;
        //WARN: equality comp with float
        if (glm::length(moveVec) != 0) {
            moveVec = moveSpeed * glm::normalize(moveVec);
        }
        player_->translate(moveVec);
        auto lookMat = glm::lookAt(camera_.getTransform().getPosition(), player_->getTransform().getPosition(), gUpVec);
        glm::quat camRot = glm::conjugate(glm::toQuat(lookMat));
        camera_.getTransform().setOrientation(camRot);
    }
    { //Camera movement
        auto moveVec = glm::vec3(0.f, 0.f, 0.f);
        F32 moveSpeed = 0.3f;
        if (keys[SDL_SCANCODE_UP]) {
            moveVec += glm::vec3(0.f, -moveSpeed, 0.f);
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            moveVec += glm::vec3(0.f, moveSpeed, 0.f);
        }
        if (keys[SDL_SCANCODE_LEFT]) {
            moveVec += glm::vec3(moveSpeed, 0.f, 0.f);
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            moveVec += glm::vec3(-moveSpeed, 0.f, 0.f);
        }
        //WARN: equality comp with float
        if (glm::length(moveVec) != 0) {
            moveVec = moveSpeed * glm::normalize(moveVec);
        }
        auto cPos = camera_.getTransform().getPosition();
        auto pPos = player_->getTransform().getPosition();
        auto desiredDist = 10.0f;
        auto actualDist = glm::length(pPos - cPos);
        moveVec[2] = -(actualDist - desiredDist) / 2;
        
        auto rot4Mat = glm::mat4_cast(camera_.getTransform().getOrientation());
        auto rot3Mat = glm::mat3(rot4Mat);
        rot3Mat[2][2] = 0.f;
        moveVec = rot3Mat * moveVec;
        
        camera_.translate(moveVec);
        auto lookMat = glm::lookAt(cPos, pPos, gUpVec);
        glm::quat camRot = glm::conjugate(glm::toQuat(lookMat));
        camera_.getTransform().setOrientation(camRot);
    }
}
void Level::logic() {
    levelScript_.runScript(guid_ + "/update.lua");
    for (auto &entity : entities_) {
        entity.update();
    }
    //collisionWorld_->computeOverlappingPairs();
    //auto overlaps = collisionWorld_->getCollisionObjectArray();
    
    if (DEBUG_DRAW_ENABLED) {
        collisionWorld_->debugDrawWorld();
    }
}
void Level::draw() const {
    for (const auto& entity : entities_) {
        gRenderManager.render(entity, camera_);
    }
    gRenderManager.renderWaterLevel(camera_);
    if (DEBUG_DRAW_ENABLED) {
        gRenderManager.renderDebug(debugDraw_, camera_);
    }
}