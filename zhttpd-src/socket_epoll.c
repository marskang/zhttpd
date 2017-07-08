#include "socket_epoll.h"
#include <stdio.h>
#include <unistd.h>

int 
sp_create() {
    return epoll_create(1024);
}

void 
sp_release(int efd) {
    close(efd);
}

int
sp_add(int efd, int fd) {
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if(epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        return 1;
    }
    return 0;
}

void
sp_del(int efd, int fd) {
    epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);
}

void
sp_del_and_close(int efd, int fd) {
    epoll_ctl(efd, EPOLL_CTL_DEL, fd, NULL);
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

/**
 *enable为false，则epoll只监听该fd的读操作，为true读写都监听
**/
void
sp_write(int efd, int fd, int enable) {
    struct epoll_event ev;
    ev.events = EPOLLIN | (enable ? EPOLLOUT : 0);
    ev.data.fd = fd;
    epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev);
}

void 
sp_nonblocking(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    if(-1 == flag) {
        return;
    }
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}