#ifndef ZHTTPD_REQUEST_H
#define ZHTTPD_REQUEST_H

struct request_head {
    char *method;
    char *context;
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
#endif