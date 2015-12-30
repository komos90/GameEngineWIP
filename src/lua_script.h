#pragma once

extern "C" {
    #include "lua.h"
}

class LuaScript {
private:
    // NOTE: Might be a good idea to have a gScriptManager.
    lua_State* state;
public:
    explicit LuaScript();
    ~LuaScript();
};