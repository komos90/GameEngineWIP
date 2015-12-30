#pragma once

#include "common_imports.h"
#include "lua_script.h"

class Level {
private:
    // Entity dies if it falls bellow this z value
    F32 deathPlaneZCoord;
    // NOTE: Declare resources used in level inside script? Like includes ?
    // NOTE: Scripting Idea: "LevelConstructor" function loads/creates needed initial level elements.
    //                       "LevelInit" function sets initial values for all initial level elements.
    //                       "LevelUpdate" function, run once per logic frame.
    // seperate files? construct.lua, (init_static.lua?), init.lua, update.lua
    // with shared state?
    LuaScript levelScript;
public:
    explicit Level();
    ~Level();
};