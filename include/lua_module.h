#ifndef LUA_MODULE_H
#define LUA_MODULE_H

#include <lua.hpp>
#include <string>
#include <vector>
#include <functional>

namespace cclua {

class LuaModule {
public:
	virtual const std::string& get_lua_name() const = 0;
	virtual const std::vector<luaL_Reg>& get_lua_register() const = 0;
	virtual int push_lua_upvalues(lua_State* L) {
		lua_pushlightuserdata(L, this);
		return 1;
	}
	virtual ~LuaModule() = default;
};

template<typename T>
class LuaModuleDef {
public:
	const std::string module_name;
	const std::vector<luaL_Reg> module_register;
	const bool is_memory_managed_by_cc;
	const std::function<T* (lua_State*, void*)> init = [this](lua_State*, void* address) -> T* {
		if(is_memory_managed_by_cc) {
			return new T();
		} else {
			return new (address) T();
		}
	};

	const std::function<void(T*)> destroy = [this](T* object) {
		if(is_memory_managed_by_cc) {
			delete object;
		} else {
			object->~T();
		}
	};

	const std::string get_metatable_name() const {
		return std::string(module_name).append(".Metatable");
	}
};

template<typename T>
class LuaModuleAdapter final : public LuaModule {
public:
	LuaModuleAdapter(const LuaModuleAdapter& source) = delete;
	~LuaModuleAdapter() = default;

	static LuaModuleAdapter<T> make(const LuaModuleDef<T>& lua_module_def) {
		return LuaModuleAdapter<T>(lua_module_def);
	}

	const std::string& get_lua_name() const override {
		return lua_module_def_.module_name;
	}

	const std::vector<luaL_Reg>& get_lua_register() const override {
		return factory_register;
	};
private:
	LuaModuleAdapter(const LuaModuleDef<T>& lua_module_def) : lua_module_def_(lua_module_def) {}
	const LuaModuleDef<T> lua_module_def_;

	const std::vector<luaL_Reg> factory_register = {
		{"new", lua_new},
		{NULL, NULL}
	};

	static LuaModuleAdapter<T>* get_adapter(lua_State* L) {
		return reinterpret_cast<LuaModuleAdapter<T>*>(lua_touserdata(L, lua_upvalueindex(1)));
	}

	static int lua_new(lua_State* L) {
		auto adapter = get_adapter(L);
		auto lua_module_def = adapter->lua_module_def_;
		if(lua_module_def.is_memory_managed_by_cc) {
			T** userdata = reinterpret_cast<T**>(lua_newuserdatauv(L, sizeof(T*), 0));
			T* object = lua_module_def.init(L, nullptr);
			*userdata = object;
		} else {
			T* userdata = reinterpret_cast<T*>(lua_newuserdatauv(L, sizeof(T), 0));
			lua_module_def.init(L, userdata);
		}

		lua_copy(L, -1, 1);
		lua_settop(L, 1);

		auto metatable_name = lua_module_def.get_metatable_name();
		int type = luaL_getmetatable(L, metatable_name.c_str());
		if(type == LUA_TNIL) {
			lua_pop(L, 1);
			luaL_newmetatable(L, metatable_name.c_str());
			lua_pushvalue(L, -1);
			lua_setfield(L, -2, "__index");
			luaL_setfuncs(L, lua_module_def.module_register.data(), 0);
			lua_pushlightuserdata(L, adapter);
			lua_pushcclosure(L, lua_delete, 1);
			lua_setfield(L, -2, "__gc");
		}
		lua_setmetatable(L, 1);
		return 1;
	}

	static int lua_delete(lua_State* L) {
		auto lua_module_def = get_adapter(L)->lua_module_def_;
		T* object = lua_module_def.is_memory_managed_by_cc
				? *reinterpret_cast<T**>(lua_touserdata(L, 1))
				: reinterpret_cast<T*>(lua_touserdata(L, 1));
		lua_module_def.destroy(object);
		return 0;
	}
};

}
#endif // LUA_MODULE_H
