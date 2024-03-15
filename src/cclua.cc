#include "cclua.h"
#include <iostream>

namespace cclua {

Lua::Lua() : L_(luaL_newstate()) {
	luaL_openlibs(L_);
}

Lua::~Lua() {
    lua_close(L_);
}

bool Lua::execute_file(const std::string& path) {
    if (luaL_loadfile(L_, path.c_str())) {
        return false;
    }
    if (pcall()) {
        return false;
    }
    return true;
}

bool Lua::execute(const std::string& script) {
    if (luaL_loadstring(L_, script.c_str())) {
        return false;
    }
    if (pcall()) {
        return false;
    }
    return true;
}

LuaValue Lua::get_value(int index) {
    switch (lua_type(L_, index)) {
    	case LUA_TNIL:
    	    return LuaNil::make();
    	case LUA_TBOOLEAN:
    	    return LuaBoolean::make(lua_toboolean(L_, index) == 1);
    	case LUA_TNUMBER:
    	    return LuaNumber::make((double)lua_tonumber(L_, index));
    	case LUA_TSTRING:
    	    return LuaString::make(lua_tostring(L_, index));
    	case LUA_TFUNCTION:
    	    return LuaFunction::make();
    	case LUA_TTABLE:
    	    return LuaTable::make();
    	default:
    	    return LuaNil::make();
    }
}

LuaValue Lua::pop_value() {
    auto value = get_value(-1);
    lua_pop(L_, 1);
    return value;
}

bool Lua::pcall(int nargs, int nresults) {
    if (lua_pcall(L_, nargs, nresults, 0)) {
        return false;
    }
    return true;
}

std::string Lua::pop_string() {
    auto result = std::get<LuaString>(pop_value());
    return result.value;
}

} // namespace cclua
