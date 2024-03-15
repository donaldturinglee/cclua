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

LuaValue Lua::get_global(const std::string& name) {
	lua_getglobal(L_, name.c_str());
	return pop_value();
}

void Lua::set_global(const std::string& name, const LuaValue& value) {
	push_value(value);
	lua_setglobal(L_, name.c_str());
}

LuaValue Lua::get_table(const std::string& table, const std::string& key) {
	int type = lua_getglobal(L_, table.c_str());
	lua_getfield(L_, -1, key.c_str());
	auto value = pop_value();
	lua_pop(L_, 1);
	return value;
}

void Lua::set_table(const std::string& table, const std::string& key, const LuaValue& value) {
	int type = lua_getglobal(L_, table.c_str());
	push_value(value);
	lua_setfield(L_, -2, key.c_str());
	lua_pop(L_, 1);
}

LuaValue Lua::get_table(const std::string& table, int index) {
	int type = lua_getglobal(L_, table.c_str());
	lua_geti(L_, -1, index);
	auto value = pop_value();
	lua_pop(L_, 1);
	return value;
}

void Lua::set_table(const std::string& table, int index, const LuaValue& value) {
	int type = lua_getglobal(L_, table.c_str());
	push_value(value);
	lua_seti(L_, -2, index);
	lua_pop(L_, 1);
}

void Lua::push_value(const LuaValue& value) {
    switch (get_lua_type(value)) {
    	case LuaType::kNil:
    	    lua_pushnil(L_);
    	    break;
    	case LuaType::kBoolean:
    	    lua_pushboolean(L_, std::get<LuaBoolean>(value).value ? 1 : 0);
    	    break;
    	case LuaType::kNumber:
    	    lua_pushnumber(L_, std::get<LuaNumber>(value).value);
    	    break;
    	case LuaType::kString:
    	    lua_pushstring(L_, std::get<LuaString>(value).value.c_str());
    	    break;
    	case LuaType::kFunction:
    	    assert(false && "pushing a Lua function is not supported");
    	    break;
    	case LuaType::kTable:
    	    assert(false && "pushing a Lua table is not supported");
    	    break;
    	case LuaType::kLightUserData:
    	    lua_pushlightuserdata(L_, std::get<LuaLightUserData>(value).value);
    	    break;
    }
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
