#pragma once

#include "EngineTypes.h"
#include "Log.h"
#include "Engine.h"

#include "Nodes/3D/Sphere3d.h"

#include "LuaBindings/Node_Lua.h"
#include "LuaBindings/LuaUtils.h"

#if LUA_ENABLED

#define SPHERE_3D_LUA_NAME "Sphere3D"
#define SPHERE_3D_LUA_FLAG "cfSphere3D"
#define CHECK_SPHERE_3D(L, arg) static_cast<Sphere3D*>(CheckNodeLuaType(L, arg, SPHERE_3D_LUA_NAME, SPHERE_3D_LUA_FLAG));

struct Sphere3D_Lua
{
    static int GetRadius(lua_State* L);
    static int SetRadius(lua_State* L);

    static void Bind();
};

#endif
