#include "cclua.h"
#include <iostream>

using namespace cclua;

int main(int argc, char* argv[]) {

	Lua lua;
	lua.execute_file("scripts/main.lua");

	std::vector<LuaValue> result = lua.vcall("foo", LuaValue(LuaNumber::make(1)), LuaValue(LuaString::make("bar")));

	if(result.size() == 2) {
		std::cout  << "Returned number: " << to_string(result[0]) << "\n";
		std::cout  << "Returned string: " << to_string(result[1]) << "\n";
	} else {
		std::cerr << "Unexpected number of return values.\n";
	}

	return 0;
}
