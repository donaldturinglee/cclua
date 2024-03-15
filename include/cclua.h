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
	LuaValue get_table(const std::string& table, const std::string& key);
	void set_table(const std::string& table, const std::string& key, const LuaValue& value);
	LuaValue get_table(const std::string& table, int index);
	void set_table(const std::string& table, int index, const LuaValue& value);
	LuaValue get_registry(const LuaValue& key);
	void set_registry(const LuaValue& key, const LuaValue& value);
	
	template<typename... Params>
	LuaValue call(const std::string& function, const Params& ...params) {
		int type = lua_getglobal(L_, function.c_str());
		for(const auto& param : std::initializer_list<LuaValue>{params...}) {
			push_value(param);
		}
		pcall(sizeof...(params), 1);
		return pop_value();
	}

	template<typename... Params>
	std::vector<LuaValue> vcall(const std::string& function, const Params& ...params) {
		int stack_size = lua_gettop(L_);
		int type = lua_getglobal(L_, function.c_str());
		for(const auto& param : std::initializer_list<LuaValue>{params...}) {
			push_value(param);
		}
		if(pcall(sizeof...(params), LUA_MULTRET)) {
			int results = lua_gettop(L_) - stack_size;
			return pop_values(results);
		}
		return std::vector<LuaValue>();
	}

private:
	bool pcall(int nargs = 0, int nresults = 0);
	void push_value(const LuaValue& value);
	LuaValue get_value(int index);
	LuaValue pop_value();
	std::vector<LuaValue> pop_values(int size);
	std::string pop_string();
	lua_State* const L_;
};

} // namespace cclua

#endif // CCLUA_H
