#pragma once

#include <vector>

#include "common_imports.h"
#include "lua_script.h"
#include "entity.h"
#include "camera.h"
#include "btBulletDynamicsCommon.h"
#include "debug_draw.h"

//main:
//  StateStack:
//      MainGameState:
//          playerScoreData
//          Level:
//              entities
class Level {
private:
    // Entity dies if it falls bellow this z value
    F32 deathPlaneZCoord_;
    std::string guid_;
    std::vector<Entity> entities_;
    Entity* player_;
    btCollisionWorld* collisionWorld_;
    mutable DebugDraw debugDraw_;
    // entities, statics, 
    // NOTE: Declare resources used in level inside script? Like includes ?
    // NOTE: Scripting Idea: "LevelConstructor" function loads/creates needed initial level elements.
    //                       "LevelInit" function sets initial values for all initial level elements.
    //                       "LevelUpdate" function, run once per logic frame.
    // seperate files? construct.lua, (init_static.lua?), init.lua, update.lua
    // with shared state?
    LuaLevelScript levelScript_;
    Camera camera_;

public:
    explicit Level(const std::string& guid);
    ~Level();

    int addEntity();
    const Entity& getEntity(int i) const;
    void setEntity(int i, const Entity& entity);
    void setPlayerEntity(U32 entityId);

    void events();
    void logic();
    void draw() const;
};