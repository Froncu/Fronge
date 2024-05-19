#include "Typenames.hpp"

#include <lua.hpp>

int main()
{
    fro::CustomUniquePointer<lua_State> pLuaState{ luaL_newstate(), lua_close };
    luaL_openlibs(pLuaState.get());
}