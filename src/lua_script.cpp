#include <SDL.h>
extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "lua_script.h"
#include "level.h"
#include "resource_manager.h"

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
    // NOTE: should be handled by gFileSystemManager
    luaL_dofile(state_, ("res/engine_ready/" + path).c_str());
}


// This template wraps a member function into a C-style "free" function compatible with lua.


int LuaLevelScript::createEntity(lua_State* state) {
    int index = level_.addEntity();
    lua_pushnumber(state_, index);
    return 1;
}
int LuaLevelScript::setMesh(lua_State* state) {
    auto i = static_cast<int>(luaL_checkinteger(state_, 1));
    auto meshPath = luaL_checkstring(state_, 2);
    auto entity = level_.getEntity(i);
    entity.setMesh(gResourceManager.getMesh(meshPath));
    level_.setEntity(i, entity);
    return 0;
}
int LuaLevelScript::setPosition(lua_State* state) {
    auto i = static_cast<int>(luaL_checkinteger(state_, 1));
    auto x = static_cast<float>(luaL_checknumber(state_, 2));
    auto y = static_cast<float>(luaL_checknumber(state_, 3));
    auto z = static_cast<float>(luaL_checknumber(state_, 4));
    auto entity = level_.getEntity(i);
    entity.setPosition(glm::vec3(x, y, z));
    level_.setEntity(i, entity);
    return 0;
}
int LuaLevelScript::clone(lua_State* state) {
    auto i = static_cast<int>(luaL_checkinteger(state_, 1));
    auto entity = level_.getEntity(i);
    auto j = level_.addEntity();
    level_.setEntity(j, entity);
    return 0;
}
LuaLevelScript::LuaLevelScript(Level& level) :
level_(level){
    // Create Lua->C Level API
    *static_cast<LuaLevelScript**>(lua_getextraspace(state_)) = this;
    lua_pushcfunction(state_, dispatch<&LuaLevelScript::createEntity>);
    lua_setglobal(state_, "create_entity");
    lua_pushcfunction(state_, dispatch<&LuaLevelScript::setMesh>);
    lua_setglobal(state_, "set_mesh");
    lua_pushcfunction(state_, dispatch<&LuaLevelScript::setPosition>);
    lua_setglobal(state_, "set_position");
    lua_pushcfunction(state_, dispatch<&LuaLevelScript::clone>);
    lua_setglobal(state_, "clone");
}
LuaLevelScript::~LuaLevelScript() {

}