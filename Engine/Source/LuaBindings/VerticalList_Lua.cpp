#include "LuaBindings/VerticalList_Lua.h"
#include "LuaBindings/Canvas_Lua.h"

#include "LuaBindings/Vector_Lua.h"
#include "LuaBindings/Rect_Lua.h"

#if LUA_ENABLED

int VerticalList_Lua::AddListItem(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);
    Widget* widget = CHECK_WIDGET(L, 2);

    list->AddListItem(widget);

    return 0;
}

int VerticalList_Lua::RemoveListItem(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);

    if (lua_isinteger(L, 2))
    {
        int32_t index = CHECK_INTEGER(L, 2);
        index--; // Convert from lua to c
        list->RemoveListItem((uint32_t)index);
    }
    else
    {
        Widget* widget = CHECK_WIDGET(L, 2);
        list->RemoveListItem(widget);
    }

    return 0;
}

int VerticalList_Lua::GetListItem(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);
    int32_t index = CHECK_INTEGER(L, 2);
    index--; // Convert from lua to c

    Widget* ret = list->GetListItem((uint32_t)index);

    Node_Lua::Create(L, ret);
    return 1;
}

int VerticalList_Lua::GetNumListItems(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);

    uint32_t ret = list->GetNumListItems();

    lua_pushinteger(L, (int)ret);
    return 1;
}

int VerticalList_Lua::SetDisplayCount(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);
    int32_t value = CHECK_INTEGER(L, 2);

    list->SetDisplayCount(value);

    return 0;
}

int VerticalList_Lua::GetDisplayCount(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);

    uint32_t ret = list->GetDisplayCount();

    lua_pushinteger(L, (int)ret);
    return 1;
}

int VerticalList_Lua::SetDisplayOffset(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);
    int32_t value = CHECK_INTEGER(L, 2);

    list->SetDisplayOffset(value);

    return 0;
}

int VerticalList_Lua::GetDisplayOffset(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);

    int32_t ret = list->GetDisplayOffset();

    lua_pushinteger(L, (int)ret);
    return 1;
}

int VerticalList_Lua::SetFitContents(lua_State* L)
{
    VerticalList* list = CHECK_VERTICAL_LIST(L, 1);
    bool value = CHECK_BOOLEAN(L, 2);

    list->SetFitContents(value);

    return 0;
}


void VerticalList_Lua::Bind()
{
    lua_State* L = GetLua();
    int mtIndex = CreateClassMetatable(
        VERTICAL_LIST_LUA_NAME,
        VERTICAL_LIST_LUA_FLAG,
        CANVAS_LUA_NAME);

    Node_Lua::BindCommon(L, mtIndex);

    REGISTER_TABLE_FUNC(L, mtIndex, AddListItem);

    REGISTER_TABLE_FUNC(L, mtIndex, RemoveListItem);

    REGISTER_TABLE_FUNC(L, mtIndex, GetListItem);

    REGISTER_TABLE_FUNC(L, mtIndex, GetNumListItems);

    REGISTER_TABLE_FUNC(L, mtIndex, SetDisplayCount);

    REGISTER_TABLE_FUNC(L, mtIndex, GetDisplayCount);

    REGISTER_TABLE_FUNC(L, mtIndex, SetDisplayOffset);

    REGISTER_TABLE_FUNC(L, mtIndex, GetDisplayOffset);

    REGISTER_TABLE_FUNC(L, mtIndex, SetFitContents);

    lua_pop(L, 1);
    OCT_ASSERT(lua_gettop(L) == 0);
}

#endif
