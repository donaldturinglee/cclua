#include "cclua.h"
#include <iostream>

using namespace cclua;

int main(int argc, char* argv[]) {

	Lua lua;
	//lua.execute_file("scripts/main.lua");
	
	lua.execute("my_table = {key = 'value', 'first', 'second'}");

	LuaValue key_value = lua.get_table("my_table", "key");
	LuaValue first_value = lua.get_table("my_table", 1);
	LuaValue second_value = lua.get_table("my_table", 2);

	std::cout << "my_table[key] = " << to_string(key_value) << "\n";
	std::cout << "my_table[1] = " << to_string(first_value) << "\n";
	std::cout << "my_table[2] = " << to_string(second_value) << "\n";

	std::cout << "\n";

	lua.set_table("my_table", "key", LuaValue(LuaString::make("new value")));
	lua.set_table("my_table", 1, LuaValue(LuaString::make("new first")));
	lua.set_table("my_table", 2, LuaValue(LuaString::make("new second")));
	
	LuaValue new_key_value = lua.get_table("my_table", "key");
	LuaValue new_first_value = lua.get_table("my_table", 1);
	LuaValue new_second_value = lua.get_table("my_table", 2);

	std::cout << "my_table[key] = " << to_string(new_key_value) << "\n";
	std::cout << "my_table[1] = " << to_string(new_first_value) << "\n";
	std::cout << "my_table[2] = " << to_string(new_second_value) << "\n";

	return 0;
}
