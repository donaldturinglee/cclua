#include "cclua.h"
#include <iostream>

int main(int argc, char* argv[]) {

	cclua::Lua lua;
	lua.execute_file("scripts/main.lua");
	
	cclua::LuaValue value = lua.get_global("whom");
	
	std::cout << "The value of 'whom' is: " << cclua::to_string(value) << "\n";
	lua.execute("hello()");

	lua.set_global("whom", cclua::LuaString::make("Lua"));

	cclua::LuaValue new_value = lua.get_global("whom");
	std::cout << "The value of 'whom' is: " << cclua::to_string(new_value) << "\n";
	lua.execute("hello()"); 
	
	return 0;
}
