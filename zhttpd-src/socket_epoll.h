#ifndef SOCKET_EPOLL_H
#define SOCKET_EPOLL_H
#include <sys/epoll.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int epollfd;

int sp_create();

void sp_release(int efd);

int sp_add(int efd, int fd);

void sp_del(int efd, int fd);

void sp_write(int efd, int fd, int enable);

void sp_nonblocking(int fd);

void sp_del_and_close(int efd, int fd);

#endif