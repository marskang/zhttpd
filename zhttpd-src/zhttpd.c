#include <stdio.h>
#include "zhttpd_request.h"
#include "zhttpd.h"
#include <stdlib.h>

int 
zhttpd_init() {
    struct sockaddr_in server_addr;
    int err = 0;
    ss = socket(AF_INET, SOCK_STREAM, 0);
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
    err = listen(ss, 10);
    if(err < 0) {
        printf("listen error\n");
        return -1;
    }
    if(err < 0) {
        printf("listen error\n");
        return -1;
    }
    return 0;
}

int 
zhttpd_start() {
    socklen_t addrlen = sizeof(struct sockaddr);
    struct sockaddr client_addr;
    int scfd = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
    if(scfd < 0) {
        return -1;
    }
    char buffer[MAX_LINE_SIZE];
    zhttpd_read_quest(scfd, buffer, MAX_LINE_SIZE);
    printf("%s\n",req_head.method);
    printf("%s\n",req_head.context);
    printf("%s\n",req_head.protocol);
    close(scfd);
    close(ss);
    return 0;
}

int 
zhttpd_read_quest(int fd, char *buffer, int len) {
    zhttpd_read_reqline(fd, buffer, len);
    zhttpd_read_header(fd, buffer, len);
    return 0;
}

int 
zhttpd_read_reqline(int fd, char *buffer, int len) {
    assert(buffer);
    memset(buffer, 0, len);
    zhttpd_read_line(fd, buffer, len);
    char str_temp[MAX_LINE_SIZE];
    strcpy(str_temp, buffer);
    char *temp, *str;
    req_head.method = (char *)malloc(MAX_LINE_SIZE);
    req_head.context = (char *)malloc(MAX_LINE_SIZE);
    req_head.protocol = (char *)malloc(MAX_LINE_SIZE);
    str = strtok_r(str_temp, " ", &temp);
    memcpy(req_head.method, str, MAX_LINE_SIZE);
    str = strtok_r(NULL, " ", &temp);
    memcpy(req_head.context, str, MAX_LINE_SIZE);
    str = strtok_r(NULL, " ", &temp);
    memcpy(req_head.protocol, str, MAX_LINE_SIZE);
    req_head.protocol[strlen(req_head.protocol) - 1] = '\0';
    return 0;
}

int 
zhttpd_read_header(int fd, char *buffer, int len) {
    ssize_t size = 0;
    for(;;) {
        size = zhttpd_read_line(fd, buffer, MAX_LINE_SIZE);
        if(strcmp(buffer, "\r\n") ==0 || size <= 0) {
            break;
        }
        char str_temp[MAX_LINE_SIZE];
        strcpy(str_temp, buffer);
        char *key, *val, *temp;
        key = strtok_r(str_temp, ":", &temp);
        val = strtok_r(NULL, ":", &temp);
        //去掉头部空格
        val++;
        //去掉末尾的\r\n
        val[strlen(val)-1] = '\0';
        zhttpd_set_head(&req_head, key, val);
    }
    return 0;
}

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

ssize_t 
zhttpd_read_line(int fd, char *buffer, int len) {
    assert(buffer);
    memset(buffer, 0, len);
    char ch = '\0';
    int i = 0;
    while(i < len-1 && ch != '\n') {
        ssize_t re = recv(fd, &ch, 1,0);
        if(re <= 0) {
            break;
        }
        buffer[i++] = ch;
    }
    buffer[i] = '\0';
    return i;
}