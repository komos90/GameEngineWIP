extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "lua_script.h"

LuaScript::LuaScript() {
    state_ = luaL_newstate();
    luaL_openlibs(state_);
#if _DEBUG
    lua_pushboolean(state_, true);
#else
    lua_pushboolean(state_, false);
#endif
    lua_setglobal(state_, "DEBUG");
}
LuaScript::~LuaScript() {
    lua_close(state_);
}
void LuaScript::runScript(const std::string& path) {
    luaL_dofile(state_, path.c_str());
}