#ifndef ZHTTPD_H
#define ZHTTPD_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "zhttpd_request.h"
#define MAX_LINE_SIZE 1024
#define ERROR -1
#define SUCCESS 1

int zhttpd_init();
int zhttpd_start();
ssize_t zhttpd_read_line(int fd, char *buffer, int len);
int zhttpd_read_header(int fd, char *buffer, int len);
int zhttpd_read_header(int fd, char *buffer, int len); 
int zhttpd_read_quest(int fd, char *buffer, int len);
int zhttpd_read_reqline(int fd, char *buffer, int len);
void zhttpd_set_head(struct request_head *head, char *key, char *val);
int ss;

#endif