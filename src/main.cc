#include "cclua.h"

int main(int argc, char* argv[]) {

	cclua::Lua lua;
	lua.execute_file("scripts/main.lua");
	//lua.execute("print('Hello C++')");

	return 0;
}
