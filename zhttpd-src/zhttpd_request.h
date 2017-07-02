#ifndef ZHTTPD_REQUEST_H
#define ZHTTPD_REQUEST_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "zhttpd.h"

struct request_head {
    int fd;
    char *method;
    char *context;
    char *query_string;
    char *protocol;
    char *host;
    char *user_agent;
    char *accept;
    char *accept_language;
    char *accept_encoding;
    char *connection;
    char *upgrade_insecure_requests;
};

struct request_head req_head;

int zhttpd_read_header(int fd, char *buffer, int len);
int zhttpd_read_quest(int fd);
int zhttpd_read_reqline(int fd, char *buffer, int len);
void zhttpd_set_head(struct request_head *head, char *key, char *val);

#endif