#ifndef ZHTTPD_RESPONSE_H
#define ZHTTPD_RESPONSE_H
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include "zhttpd_request.h"

struct zhttpd_resp {
    int fd;
};


void zhttpd_start_resp(struct request_head *req_head);
int zhttpd_set_lua_querystring(char *query_string, int fd);
void resp_400(int fd);

void resp_404(int fd);

void resp_501(int fd);

void resp_500(int fd);

void resp_200(int fd);

/** 用于lua调用 */
int resp_write(lua_State *L); 


#endif