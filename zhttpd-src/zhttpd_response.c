#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
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
 
void 
zhttpd_start_service(struct request_head *req_head) {
    char *context = req_head->context;
    printf("context:%s\n", context);
    int fd = req_head->fd;
    char path[256] = "htdocs";
    int len = strlen(context);
    if(!strcasecmp(context, "/")) {
        strcat(path, "/index.html");
        printf("path:%s\n", path);
    } else {
        if(context[len-1] == '/') {
            context[--len] = '\0';
        }
        char suffix[4];
        suffix[4] = '\0';
        int i = 0;
        strcat(path, context);
        if(len > 4) {
            while(i <= 2) {
                suffix[i] = context[len-3+i];
                i++;
            }
            if(!strcasecmp(suffix, "lua")) {
                zhttpd_set_lua_service(path, req_head->query_string, fd);
                return;
            }
        }
        printf("path:%s\n", path);
    }
    zhttpd_resp_file(path, fd);
}



void 
zhttpd_resp_file(char *path, int fd) {
    int tfd = -1;
    tfd = open(path, O_RDONLY);
    if(-1 == tfd) {
        resp_404(fd);
        return;
    }
    resp_200(fd);
    ssize_t size = -1;
    char buffer[1024];
    while(size) {
        size = read(tfd, buffer, 1024);
        if(-1 == size) {
            resp_500(fd);
            close(tfd);
            return;
        }
        if(size > 0) {
            write(fd, buffer, strlen(buffer));
        }
    }
}

int
zhttpd_set_lua_service(char *req_file,char *query_string, int fd) {
    char *filename = "service/service.lua";
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    if(luaL_dofile(L, filename) || lua_pcall(L, 0,0,0)) {
        printf("error 1,%s\n", lua_tostring(L, -1));
    }
    lua_getglobal(L, "service");
    lua_pushstring(L, req_file);
    lua_pushstring(L, query_string);
    lua_pushnumber(L, fd);
    lua_pcall(L, 3, 1, 0);
    lua_close(L);
    return 0;
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
    const char *str = 
        "HTTP/1.1 200 OK\r\n" 
        "Server: zhttpd\r\n"
        "Content-Type: text/html\r\n"
        "\r\n";
    write(fd, str, strlen(str));  
}