#ifndef ZHTTPD_LIB_SERVICE_H
#define ZHTTPD_LIB_SERVICE_H

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

int resp_write(lua_State *L);

int lua_resp_400(lua_State *L);

int lua_resp_404(lua_State *L);

int lua_resp_501(lua_State *L);

int lua_resp_500(lua_State *L);

#endif