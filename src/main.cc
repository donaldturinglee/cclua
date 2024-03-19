#include "cclua.h"
#include <iostream>

using namespace cclua;

class HelloWorld {
public:
    HelloWorld() : message("Hello, World!") {}
    std::string message;
};

int hello_message(lua_State* L) {
    HelloWorld* hello_world = reinterpret_cast<HelloWorld*>(luaL_checkudata(L, 1, "HelloWorld.Metatable"));
    lua_pushstring(L, hello_world->message.c_str());
    return 1;
}

int main(int argc, char* argv[]) {

	Lua lua;

	LuaModuleDef<HelloWorld> hello_world_def = {
		"HelloWorld",
		{{"message", hello_message}, {NULL, NULL}},
		false
	};

	auto hello_world_module = LuaModuleAdapter<HelloWorld>::make(hello_world_def);

	lua.register_module(hello_world_module);
	lua.execute("hello_world = HelloWorld.new()\nprint(hello_world:message())");

	return 0;
}
