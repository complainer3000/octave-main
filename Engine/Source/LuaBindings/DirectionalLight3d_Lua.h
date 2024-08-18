#pragma once

#include "EngineTypes.h"
#include "Log.h"
#include "Engine.h"

#include "Nodes/3D/DirectionalLight3d.h"

#include "LuaBindings/Node_Lua.h"
#include "LuaBindings/LuaUtils.h"

#if LUA_ENABLED

#define DIRECTIONAL_LIGHT_3D_LUA_NAME "DirectionalLight3D"
#define DIRECTIONAL_LIGHT_3D_LUA_FLAG "cfDirectionalLight3D"
#define CHECK_DIRECTIONAL_LIGHT_3D(L, arg) static_cast<DirectionalLight3D*>(CheckNodeLuaType(L, arg, DIRECTIONAL_LIGHT_3D_LUA_NAME, DIRECTIONAL_LIGHT_3D_LUA_FLAG));

struct DirectionalLight3D_Lua
{
    static int GetDirection(lua_State* L);
    static int SetDirection(lua_State* L);

    static void Bind();
};

#endif
