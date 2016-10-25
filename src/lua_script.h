#pragma once

#include <string>

extern "C" {
    #include "lua.h"
}

class Level;

class LuaScript {
protected:
    // NOTE: Might be a good idea to have a gScriptManager.
    lua_State* state_;
public:
    explicit LuaScript();
    ~LuaScript();
    void runScript(const std::string& path);
};

class LuaLevelScript;
typedef int (LuaLevelScript::*memFunc)(lua_State * L);

class LuaLevelScript : public LuaScript {
private:
    Level& level_;
    int x = 6;
public:
    explicit LuaLevelScript(Level& level);
    ~LuaLevelScript();

    template <memFunc func>
    static int dispatch(lua_State* L);
    int createEntity(lua_State* state);
    int setMesh(lua_State* state);
    int setPosition(lua_State* state);
    int addPosition(lua_State* state);
    int clone(lua_State* state);
};

template <memFunc func>
int LuaLevelScript::dispatch(lua_State* L) {
    LuaLevelScript * ptr = *static_cast<LuaLevelScript**>(lua_getextraspace(L));
    return ((*ptr).*func)(L);
}