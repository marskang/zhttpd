#include <stdio.h>
#include "zhttpd_request.h"
#include "zhttpd_response.h"
#include "zhttpd.h"
#include "socket_epoll.h"
#include <stdlib.h>

int 
zhttpd_init() {
    struct sockaddr_in server_addr;
    int err = 0;
    ss = socket(AF_INET, SOCK_STREAM, 0);
    sp_nonblocking(ss);
    if(ss < 0) {
        //TODO 以后这里用log输出
        printf("socket error\n");
        return -1;
    }
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    //TODO 配置到配置文件中
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    server_addr.sin_port = htons(8080);
    err = bind(ss, (struct sockaddr *)&server_addr,sizeof(server_addr));
    if(err < 0) {
        printf("bind error\n");
        return -1;
    }
    listen(ss, 10);
    if(err < 0) {
        printf("listen error\n");
        return -1;
    }
    return ss;
}

int 
zhttpd_start(int listenfd) {
    epollfd = sp_create();
    struct epoll_event events[100];
    sp_add(epollfd, listenfd);
    for( ; ; ) {
        int num = epoll_wait(epollfd, events, 100, -1);
        printf("num:%d\n", num);
        int i = 0;
        for(i = 0; i < num; i++) {
            int fd = events[i].data.fd;
            if((fd == listenfd) && (events[i].events & EPOLLIN)) {
                printf("fd:%d,accept\n", fd);
                handle_accept(epollfd, listenfd);
            } else if(events[i].events & EPOLLIN) {
                printf("fd:%d,epollin\n", fd);
                zhttpd_read_quest(fd);
                sp_del_and_close(epollfd, fd);
            } else if(events[i].events & EPOLLOUT) {
                printf("fd:%d,out\n", fd);
                sp_del_and_close(epollfd, fd);
                shutdown(fd, SHUT_RDWR);
                close(fd);
            } else {
                sp_del_and_close(epollfd, fd);
                shutdown(fd, SHUT_RDWR);
                close(fd);
            }
        }
    }
    sp_release(epollfd);
    close(ss);
    return 0;
}

void handle_accept(int epollfd, int listenfd) {
    int fd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    fd = accept(listenfd, (struct sockaddr*)&addr, &addrlen);
    if(-1 == fd) {
        printf("accept error\n");
    } else {
        printf("accept a new client\n");
        sp_add(epollfd, fd);
    }
}

ssize_t 
zhttpd_read_line(int fd, char *buffer, int len) {
    assert(buffer);
    memset(buffer, 0, len);
    char ch = '\0';
    int i = 0;
    while(i < len-1 && ch != '\n') {
        ssize_t re = recv(fd, &ch, 1,0);
        if(re <= 0) {
            return re;
        }
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    //printf("%s\n", buffer);
    return i;
}