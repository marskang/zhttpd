#include "lua_request.h"

/*int
set_lua_request_querystring(lua_State *L, char *query_string) {
    lua_getglobal(L, "fun");
    lua_pushstring(L, query_string);
    lua_call(L,1,0);
    // lua_pop(L,-1);
    return 1;
}*/

int 
set_table_test(lua_State *L) {
    lua_newtable(L);
    int i;
    char value[10] = {0};
    for(i = 0; i < 5; i++) {
        sprintf(value, "value%d", i+1);
        lua_pushnumber(L, i+1);
        lua_pushstring(L, value);
        lua_settable(L, -3);
    }
    return 1;
}