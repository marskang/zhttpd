#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "zhttpd_response.h"

ssize_t
zhttpd_write(struct zhttpd_resp resp, char *buffer) {
    assert(buffer);
    ssize_t size = -1;
    size = write(resp.fd, buffer, strlen(buffer));   
    return size;
}


static luaL_Reg mylibs[] = {
    { "resp_write", resp_write },
    { NULL , NULL }
};

int 
resp_write(lua_State *L) {
    int fd =  lua_tonumber(L, 1);
    const char *str = lua_tostring(L, 2);
    ssize_t size = write(fd, str, strlen(str));
    lua_pushnumber(L, size);
    return 1;
}
 
void 
zhttpd_start_resp(struct request_head *req_head) {
    char *context = req_head->context;
    int fd = req_head->fd;
    zhttpd_set_lua_querystring(req_head->query_string, fd);
}

int
zhttpd_set_lua_querystring(char *query_string, int fd) {
    char *filename = "service/request.lua";
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if(luaL_dofile(L, filename) || lua_pcall(L, 0,0,0)) {
        printf("error 1,%s\n", lua_tostring(L, -1));
    }
    lua_getglobal(L, "set_querystring");
    lua_pushstring(L, query_string);
    lua_pushnumber(L, fd);
    lua_pcall(L, 2, 1, 0);
    lua_close(L);
    return 0;
}

int
luaopen_libresp(lua_State *L) {
    luaL_newlib(L, mylibs);
    return 1;
}

inline void 
resp_400(int fd) {
    const char *str = 
        "HTTP/1.1 400 BAD REQUEST\r\n" 
        "Server: zhttpd\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html>"
        "<head><title>BAD REQUEST</title></head>"
        "<body><p>BAD REQUEST</p></body>"
        "</html>";
    write(fd, str, strlen(str));
}

inline void 
resp_404(int fd) {
    const char *str = 
        "HTTP/1.1 404 NOT FOUND\r\n" 
        "Content-Type: text/html\r\n"
        "Server: zhttpd\r\n"
        "\r\n"
        "<html>"
        "<head><title>NOT FOUND</title></head>"
        "<body><p>NOT FOUND</p></body>"
        "</html>";
    write(fd, str, strlen(str));
}

inline void 
resp_501(int fd) {
    const char *str = 
        "HTTP/1.1 501 Method Not Implemented\r\n" 
        "Server: zhttpd\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html>"
        "<head><title>Method Not Implemented</title></head>"
        "<body><p>Method Not Implemented</p></body>"
        "</html>";
    write(fd, str, strlen(str));
}

inline void
resp_500(int fd) {
    const char *str = 
        "HTTP/1.1 500 Internal Server Error\r\n" 
        "Server: zhttpd\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html>"
        "<head><title>Internal Server Error</title></head>"
        "<p>Internal Server Error</p>"
        "</html>";
    write(fd, str, strlen(str));  
}

inline void 
resp_200(int fd) {

}