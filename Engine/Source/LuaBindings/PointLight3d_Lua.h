#pragma once

#include "EngineTypes.h"
#include "Log.h"
#include "Engine.h"

#include "Nodes/3D/PointLight3d.h"

#include "LuaBindings/Node_Lua.h"
#include "LuaBindings/LuaUtils.h"

#if LUA_ENABLED

#define POINT_LIGHT_3D_LUA_NAME "PointLight3D"
#define POINT_LIGHT_3D_LUA_FLAG "cfPointLight3D"
#define CHECK_POINT_LIGHT_3D(L, arg) static_cast<PointLight3D*>(CheckNodeLuaType(L, arg, POINT_LIGHT_3D_LUA_NAME, POINT_LIGHT_3D_LUA_FLAG));

struct PointLight3D_Lua
{
    static int SetRadius(lua_State* L);
    static int GetRadius(lua_State* L);

    static void Bind();
};

#endif
