#ifndef ZHTTPD_LUA_REQUEST_H
#define ZHTTPD_LUA_REQUEST_H
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include "../zhttpd-src/zhttpd_request.h"
//设置参数的函数
//设置协议的函数
int 
set_lua_request_head(struct request_head head);

int
set_lua_request_querystring(lua_State *L);

int set_table_test(lua_State *L);

#endif