CFLAGS = -g -Wall
ZHTTPD := \
zhttpd-src/zhttpd.c \
zhttpd-src/zhttpd_main.c

SRC := $(ZHTTPD)

TARGET = zhttpd

CC=gcc

zhttpd :
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean :
	rm -rf zhttpd