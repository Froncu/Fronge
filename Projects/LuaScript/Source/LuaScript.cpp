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

#if not defined NDEBUG
	auto const message{ std::format("NPC named {} added!", NPCName) };
	fro::Console::getInstance().log(message, fro::Console::BackgroundColor::darkGreen);
#endif

	return 0;
}

static int removeNPC(lua_State* const pState)
{
	auto const NPCName{ lua_tostring(pState, 1) };
	NPCManager::getInstance().removeNPC(NPCName);

#if not defined NDEBUG
	auto const message{ std::format("NPC named {} removed!", NPCName) };
	fro::Console::getInstance().log(message, fro::Console::BackgroundColor::darkRed);
#endif

	return 0;
}

int main()
{
	fro::CustomUniquePointer<lua_State> pState{ luaL_newstate(), lua_close };
	luaL_openlibs(pState.get());

	lua_register(pState.get(), "addNPC", addNPC);
	lua_register(pState.get(), "removeNPC", removeNPC);

	luaL_dofile(pState.get(), "Resources/startup.lub");

#if not defined NDEBUG
	std::cout << '\n';
#endif

	NPCManager::getInstance().dump();
}