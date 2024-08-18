#include "LuaBindings/PointLight3d_Lua.h"
#include "LuaBindings/Light3d_Lua.h"
#include "LuaBindings/Vector_Lua.h"
#include "LuaBindings/LuaUtils.h"

#if LUA_ENABLED

int PointLight3D_Lua::SetRadius(lua_State* L)
{
    PointLight3D* comp = CHECK_POINT_LIGHT_3D(L, 1);
    float value = CHECK_NUMBER(L, 2);

    comp->SetRadius(value);

    return 0;
}

int PointLight3D_Lua::GetRadius(lua_State* L)
{
    PointLight3D* comp = CHECK_POINT_LIGHT_3D(L, 1);

    float ret = comp->GetRadius();

    lua_pushnumber(L, ret);
    return 1;
}

void PointLight3D_Lua::Bind()
{
    lua_State* L = GetLua();
    int mtIndex = CreateClassMetatable(
        POINT_LIGHT_3D_LUA_NAME,
        POINT_LIGHT_3D_LUA_FLAG,
        LIGHT_3D_LUA_NAME);

    Node_Lua::BindCommon(L, mtIndex);

    REGISTER_TABLE_FUNC(L, mtIndex, SetRadius);

    REGISTER_TABLE_FUNC(L, mtIndex, GetRadius);

    lua_pop(L, 1);
    OCT_ASSERT(lua_gettop(L) == 0);
}

#endif
