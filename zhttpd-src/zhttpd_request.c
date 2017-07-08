#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "zhttpd_request.h"
#include "zhttpd_response.h"

int 
zhttpd_read_quest(int fd) {
    char *buffer;
    buffer = (char *)malloc(MAX_LINE_SIZE);
    int re = zhttpd_read_reqline(fd, buffer, MAX_LINE_SIZE);
    if(re <= 0) return re;
    if(!strcasecmp("GET", req_head.method)) {
        char *temp;
        char *str = strtok_r(req_head.context, "?", &temp);
        if(str == NULL) return -1;
        memcpy(req_head.context, str, MAX_LINE_SIZE);
        req_head.query_string = (char *)malloc(MAX_LINE_SIZE);
        str = strtok_r(NULL, "?", &temp);
        if(str == NULL) str = "";
        memcpy(req_head.query_string , str, MAX_LINE_SIZE);
    }
    re = zhttpd_read_header(fd, buffer, MAX_LINE_SIZE);
    if(re <= 0) return re;
    req_head.fd = fd;
    zhttpd_start_service(&req_head);
    return 0;
}


int 
zhttpd_read_reqline(int fd, char *buffer, int len) {
    assert(buffer);
    memset(buffer, 0, len);
    int re = zhttpd_read_line(fd, buffer, len);
    if(re <= 0) return re;
    char str_temp[MAX_LINE_SIZE];
    strncpy(str_temp, buffer, sizeof(str_temp) - 1);
    char *temp, *str;
    req_head.method = (char *)malloc(MAX_LINE_SIZE);
    req_head.context = (char *)malloc(MAX_LINE_SIZE);
    req_head.protocol = (char *)malloc(MAX_LINE_SIZE);
    str = strtok_r(str_temp, " ", &temp);
    if(str == NULL) return -1;
    memcpy(req_head.method, str, MAX_LINE_SIZE);
    str = strtok_r(NULL, " ", &temp);
    if(str == NULL) return -1;
    memcpy(req_head.context, str, MAX_LINE_SIZE);
    str = strtok_r(NULL, " ", &temp);
    if(str == NULL) return -1;
    memcpy(req_head.protocol, str, MAX_LINE_SIZE);
    return re;
}

int 
zhttpd_read_header(int fd, char *buffer, int len) {
    ssize_t size = 0;
    for(;;) {
        size = zhttpd_read_line(fd, buffer, MAX_LINE_SIZE);
        if(size <= 0) return size;
        if(strcmp(buffer, "\r\n") ==0 || size <= 0) {
            break;
        }
        char str_temp[MAX_LINE_SIZE];
        strncpy(str_temp, buffer, sizeof(str_temp) - 1);
        char *key, *val, *temp;
        key = strtok_r(str_temp, ":", &temp);
        val = strtok_r(NULL, ":", &temp);
        if(key == NULL || val == NULL) {
            return -1;
        }
        //去掉行首的空格
        while(!isspace(val[0])) {
            val++;
        }
        val[strlen(val)-1] = '\0';
        while(!isspace(key[0])) {
            key++;
        }
        key[strlen(key) - 1] = '\0';
        zhttpd_set_head(&req_head, key, val);
    }
    return size;
}


/**
 * 设置http协议的请求头
**/
void
zhttpd_set_head(struct request_head *head, char *key, char *val) {
    if(!strcasecmp(key, "host")) {
        head->host = (char *)malloc(MAX_LINE_SIZE);
        assert(head->host);
        memcpy(head->host, val, MAX_LINE_SIZE);
    }
    if(!strcasecmp(key, "User-Agent")) {
        head->user_agent = (char *)malloc(MAX_LINE_SIZE);
        assert(head->user_agent);
        memcpy(head->user_agent, val, MAX_LINE_SIZE);
    }
    if(!strcasecmp(key, "Accept")) {
        head->accept = (char *)malloc(MAX_LINE_SIZE);
        assert(head->accept);
        memcpy(head->accept, val, MAX_LINE_SIZE);
    }
    if(!strcasecmp(key, "Accept-Language")) {
        head->accept_language = (char *)malloc(MAX_LINE_SIZE);
        assert(head->accept_language);
        memcpy(head->accept_language, val, MAX_LINE_SIZE);
    }
    if(!strcasecmp(key, "Accept-Encoding")) {
        head->accept_encoding = (char *)malloc(MAX_LINE_SIZE);
        assert(head->accept_encoding);
        memcpy(head->accept_encoding, val, MAX_LINE_SIZE);
    }
    if(!strcasecmp(key, "Connection")) {
        head->connection = (char *)malloc(MAX_LINE_SIZE);
        assert(head->connection);
        memcpy(head->connection, val, MAX_LINE_SIZE);
    }
    if(!strcasecmp(key, "Upgrade-Insecure-Requests")) {
        head->upgrade_insecure_requests = (char *)malloc(MAX_LINE_SIZE);
        assert(head->upgrade_insecure_requests);
        memcpy(head->upgrade_insecure_requests, val, MAX_LINE_SIZE);
    }
}
