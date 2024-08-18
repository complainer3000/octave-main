#include "LuaBindings/DirectionalLight3d_Lua.h"
#include "LuaBindings/Light3d_Lua.h"
#include "LuaBindings/Vector_Lua.h"
#include "LuaBindings/LuaUtils.h"

#if LUA_ENABLED

int DirectionalLight3D_Lua::GetDirection(lua_State* L)
{
    DirectionalLight3D* comp = CHECK_DIRECTIONAL_LIGHT_3D(L, 1);

    glm::vec3 ret = comp->GetDirection();

    Vector_Lua::Create(L, ret);
    return 1;
}

int DirectionalLight3D_Lua::SetDirection(lua_State* L)
{
    DirectionalLight3D* comp = CHECK_DIRECTIONAL_LIGHT_3D(L, 1);
    glm::vec3 value = CHECK_VECTOR(L, 2);

    comp->SetDirection(value);

    return 0;
}

void DirectionalLight3D_Lua::Bind()
{
    lua_State* L = GetLua();
    int mtIndex = CreateClassMetatable(
        DIRECTIONAL_LIGHT_3D_LUA_NAME,
        DIRECTIONAL_LIGHT_3D_LUA_FLAG,
        LIGHT_3D_LUA_NAME);

    Node_Lua::BindCommon(L, mtIndex);

    REGISTER_TABLE_FUNC(L, mtIndex, GetDirection);

    REGISTER_TABLE_FUNC(L, mtIndex, SetDirection);

    lua_pop(L, 1);
    OCT_ASSERT(lua_gettop(L) == 0);
}

#endif
