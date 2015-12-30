#pragma once

#include <string>

extern "C" {
    #include "lua.h"
}

class LuaScript {
private:
    // NOTE: Might be a good idea to have a gScriptManager.
    lua_State* state_;
public:
    explicit LuaScript();
    ~LuaScript();
    void runScript(const std::string& path);
};