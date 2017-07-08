#include "zhttpd_lib_service.h"
#include "zhttpd_response.h"

static luaL_Reg mylibs[] = {
    { "resp_write", resp_write },
    { "resp_400", lua_resp_400 },
    { "resp_404", lua_resp_404 },
    { "resp_501", lua_resp_501 },
    { "resp_500", lua_resp_500 },
    { NULL , NULL }
};

int 
resp_write(lua_State *L) {
    int fd =  lua_tonumber(L, 1);
    const char *str = lua_tostring(L, 2);
    if(str == NULL || strlen(str) <= 0) {
        lua_pushnumber(L, -1);
        return 1;
    } 
    printf("%s\n", str);
    ssize_t size = write(fd, str, strlen(str));
    lua_pushnumber(L, size);
    return 1;
}


int lua_resp_400(lua_State *L) {
    int fd =  lua_tonumber(L, 1);
    resp_400(fd);
    return 0;
}

int lua_resp_404(lua_State *L) {
    int fd =  lua_tonumber(L, 1);
    resp_404(fd);
    return 0;
}

int lua_resp_501(lua_State *L) {
    int fd =  lua_tonumber(L, 1);
    resp_501(fd);
    return 0;
}

int lua_resp_500(lua_State *L) {
    int fd =  lua_tonumber(L, 1);
    resp_500(fd);
    return 0;
}

int
luaopen_libservice(lua_State *L) {
    luaL_newlib(L, mylibs);
    return 1;
}

