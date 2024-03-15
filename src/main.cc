#include "cclua.h"
#include <iostream>

using namespace cclua;

int main(int argc, char* argv[]) {

	Lua lua;
	lua.execute_file("scripts/main.lua");
	
	auto result = lua.resume("squares");
	if(get_lua_type(result) == LuaType::kNumber) {
		std::cout << "Coroutine yields " << std::get<LuaNumber>(result).value << "\n";
	}

	return 0;
}
