#include "LuaBindings/Capsule3d_Lua.h"
#include "LuaBindings/Primitive3d_Lua.h"
#include "LuaBindings/Vector_Lua.h"
#include "LuaBindings/Asset_Lua.h"
#include "LuaBindings/LuaUtils.h"

#if LUA_ENABLED

int Capsule3D_Lua::GetHeight(lua_State* L)
{
    Capsule3D* comp = CHECK_CAPSULE_3D(L, 1);

    float ret = comp->GetHeight();

    lua_pushnumber(L, ret);
    return 1;
}

int Capsule3D_Lua::SetHeight(lua_State* L)
{
    Capsule3D* comp = CHECK_CAPSULE_3D(L, 1);
    float value = CHECK_NUMBER(L, 2);

    comp->SetHeight(value);

    return 0;
}

int Capsule3D_Lua::GetRadius(lua_State* L)
{
    Capsule3D* comp = CHECK_CAPSULE_3D(L, 1);

    float ret = comp->GetRadius();

    lua_pushnumber(L, ret);
    return 1;
}

int Capsule3D_Lua::SetRadius(lua_State* L)
{
    Capsule3D* comp = CHECK_CAPSULE_3D(L, 1);
    float value = CHECK_NUMBER(L, 2);

    comp->SetRadius(value);

    return 0;
}

void Capsule3D_Lua::Bind()
{
    lua_State* L = GetLua();
    int mtIndex = CreateClassMetatable(
        CAPSULE_3D_LUA_NAME,
        CAPSULE_3D_LUA_FLAG,
        PRIMITIVE_3D_LUA_NAME);

    Node_Lua::BindCommon(L, mtIndex);

    REGISTER_TABLE_FUNC(L, mtIndex, GetHeight);

    REGISTER_TABLE_FUNC(L, mtIndex, SetHeight);

    REGISTER_TABLE_FUNC(L, mtIndex, GetRadius);

    REGISTER_TABLE_FUNC(L, mtIndex, SetRadius);

    lua_pop(L, 1);
    OCT_ASSERT(lua_gettop(L) == 0);
}

#endif
