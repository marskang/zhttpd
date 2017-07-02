#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include "zhttpd.h"
#include "../lualib-src/lua_request.h"

int main(int argc, char **argv) {
    int err = zhttpd_init();
    if(err < 0) {
        printf("init err\n");
        return -1;
    }
    err = zhttpd_start(err);
    if(err < 0) {
        printf("start error\n");
        return -1;
    }
    return 0;
}