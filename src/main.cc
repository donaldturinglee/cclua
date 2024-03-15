#include "cclua.h"
#include <iostream>

using namespace cclua;

int main(int argc, char* argv[]) {

	Lua lua;
	lua.execute_file("scripts/main.lua");

	LuaValue result = lua.tcall("my_table", "add", true, LuaValue(LuaNumber::make(2)), LuaValue(LuaNumber::make(3)));

	std::cout << "Result of add: " << to_string(result) << "\n";

	return 0;
}
