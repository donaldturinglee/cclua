#include "cclua.h"
#include <iostream>

using namespace cclua;

int main(int argc, char* argv[]) {

	Lua lua;

	lua.execute("function add(x, y) return x + y end");
	LuaValue result = lua.call("add", LuaValue(LuaNumber::make(1)), LuaValue(LuaNumber::make(2)));

	std::cout << "The result is: " << to_string(result) << "\n";
	
	return 0;
}
