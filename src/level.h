#pragma once

#include <vector>

#include "common_imports.h"
#include "lua_script.h"
#include "entity.h"
#include "camera.h"

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
    std::vector<Entity*> entities_;
    // entities, statics, 
    // NOTE: Declare resources used in level inside script? Like includes ?
    // NOTE: Scripting Idea: "LevelConstructor" function loads/creates needed initial level elements.
    //                       "LevelInit" function sets initial values for all initial level elements.
    //                       "LevelUpdate" function, run once per logic frame.
    // seperate files? construct.lua, (init_static.lua?), init.lua, update.lua
    // with shared state?
    LuaScript levelScript_;

    // TMP
    Camera camera_;
    const Mesh* test_;
    Entity testMonkey1_;
    
    // ENDTMP

public:
    explicit Level();
    ~Level();
    void events();
    void logic();
    void draw() const;
};