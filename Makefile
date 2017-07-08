CFLAGS = -g -Wall -I./3rd
LDFLAGS :=
ZHTTPD := \
zhttpd-src/zhttpd.c \
zhttpd-src/zhttpd_main.c \
zhttpd-src/zhttpd_request.c \
zhttpd-src/socket_epoll.c \
zhttpd-src/zhttpd_response.c \
lualib-src/lua_request.c 

SRC := $(ZHTTPD)

LUALIBPATH := service

TARGET = zhttpd

LDFLAGS += -L. -lservice

LDFLAGS += -L./3rd/lua -llua -lm -ldl

CC=gcc

all : \
libservice.so \
zhttpd

libservice.so : zhttpd-src/zhttpd_lib_service.c
	$(CC) $(CFLAGS) -shared -fPIC  $^ -o $@ 

zhttpd :
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean :
	rm -rf zhttpd
	rm -rf *.so