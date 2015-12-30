extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "lua_script.h"

LuaScript::LuaScript() {
    state = luaL_newstate();
    luaL_openlibs(state);
#if _DEBUG
    lua_pushboolean(state, true);
#else
    lua_pushboolean(state, false);
#endif
    lua_setglobal(state, "DEBUG");
    luaL_dofile(state, "res/engine_ready/level1/test.lua");
}
LuaScript::~LuaScript() {
    lua_close(state);
}