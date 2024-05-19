#include "Console.hpp"
#include "NPCManager.h"
#include "Typenames.hpp"

#include <lua.hpp>

#include <iostream>
#include <format>

static int addNPC(lua_State* const pState)
{
	auto const NPCName{ lua_tostring(pState, 1) };
	NPCManager::getInstance().addNPC(NPCName);
	return 0;
}

static int removeNPC(lua_State* const pState)
{
	auto const NPCName{ lua_tostring(pState, 1) };
	NPCManager::getInstance().removeNPC(NPCName);
	return 0;
}

static int setNPCHealth(lua_State* const pState)
{
	auto const NPCName{ lua_tostring(pState, 1) };
	auto const NPCNewHealth{ static_cast<int>(lua_tointeger(pState, 2)) };

	NPCManager::getInstance().setHealth(NPCName, NPCNewHealth);

	return 0;
}

int main()
{
	fro::CustomUniquePointer<lua_State> pState{ luaL_newstate(), lua_close };
	luaL_openlibs(pState.get());

	lua_register(pState.get(), "addNPC", addNPC);
	lua_register(pState.get(), "removeNPC", removeNPC);
	lua_register(pState.get(), "setNPCHealth", setNPCHealth);

	luaL_dofile(pState.get(), "Resources/startup.lub");
	luaL_dofile(pState.get(), "Resources/health_tweaker.lua");

#if not defined NDEBUG
	std::cout << '\n';
#endif

	NPCManager::getInstance().dump();
}