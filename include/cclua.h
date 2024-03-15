#ifndef CCLUA_H
#define CCLUA_H

#include "lua_type.h"

#include <lua.hpp>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

namespace cclua {

class Lua {
public:
	Lua();
	virtual ~Lua();
	bool execute_file(const std::string& path);
    bool execute(const std::string& script);
	LuaValue get_global(const std::string& name);
	void set_global(const std::string& name, const LuaValue& value);
private:
	bool pcall(int nargs = 0, int nresults = 0);
	void push_value(const LuaValue& value);
	LuaValue get_value(int index);
	LuaValue pop_value();
	std::string pop_string();
	lua_State* const L_;
};

} // namespace cclua

#endif // CCLUA_H
