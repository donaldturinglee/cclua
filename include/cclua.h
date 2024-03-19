#ifndef CCLUA_H
#define CCLUA_H


#include <lua.hpp>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>

#include "lua_type.h"
#include "lua_module.h"

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

	template<typename... Params>
	LuaValue tcall(const std::string& table, const std::string& function, bool include_self, const Params& ...params) {
		int type = lua_getglobal(L_, table.c_str());
		type = lua_getfield(L_, -1, function.c_str());
		if(include_self) {
			lua_getglobal(L_, table.c_str());
		}

		for(const auto& param : std::initializer_list<LuaValue>{params...}) {
			push_value(param);
		}
		int params_size = sizeof...(params) + (include_self ? 1 : 0);
		pcall(params_size, 1);

		auto result = pop_value();
		lua_pop(L_, 1);

		return result;
	}

	template<typename... Params>
	std::vector<LuaValue> vtcall(const std::string& table, const std::string& function, bool include_self, const Params& ...params) {
		int type = lua_getglobal(L_, table.c_str());
		int stack_size = lua_gettop(L_);
		type = lua_getfield(L_, -1, function.c_str());
		if(include_self) {
			lua_getglobal(L_, table.c_str());
		}
		for(const auto& param : std::initializer_list<LuaValue>{params...}) {
			push_value(param);
		}

		auto results = std::vector<LuaValue>();
		int params_size = sizeof...(params) + (include_self ? 1 : 0);
		if(pcall(params_size, LUA_MULTRET)) {
			int results_size = lua_gettop(L_) - stack_size;
			results = pop_values(results_size);
		}
		lua_pop(L_, 1);
		return results;
	}

	inline LuaValue resume(const std::string& function) {
		lua_State* t = lua_newthread(L_);
		int type = lua_getglobal(t, function.c_str());

		int results_size = 0;
		const int status = lua_resume(t, L_, 0, &results_size);

		if(status != LUA_OK && status != LUA_YIELD) {
			std::cout << "[cclua::resume] " << lua_tostring(t, -1) << "\n";
			lua_pop(t, 1);
			return LuaNil::make();
		}

		if(results_size == 0) {
			return LuaNil::make();
		}

		const int value = lua_tointeger(t, -1);
		lua_pop(t, results_size);
		lua_pop(L_, 1);
		return LuaNumber::make(value);
	}

	void register_module(LuaModule& module);
	static const int version = 1;

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
